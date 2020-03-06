#include "clang/AST/ASTConsumer.h"
#include "clang/AST/Mangle.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/CodeGen/ObjectFilePCHContainerOperations.h"
#include "clang/Driver/Options.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Frontend/FixItRewriter.h"
#include "clang/Rewrite/Frontend/FrontendActions.h"
#include "clang/StaticAnalyzer/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Option/OptTable.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"

using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static cl::extrahelp MoreHelp(
  "\tTest string please ignore\n"
  "\n"
);

static cl::OptionCategory EmImportCategory("em-import options");
static const opt::OptTable &Options = getDriverOptTable();
static cl::opt<std::string>
  Outfile("o", cl::desc("Output file"), cl::cat(EmImportCategory));

namespace {
using namespace clang;

template <typename T>
void write_vec(std::vector<T> const& vec, llvm::raw_ostream &os) {
  os.write(vec.size()); // TODO: LEB
  for (auto &elem : vec) {
    elem.write(os);
  }
}
void write_str(std::string const& str, llvm::raw_ostream &os) {
  os.write(str.size()); // TODO: LEB
  os.write(str.c_str(), str.size());
}

enum ITTypeKind {
  IT_u8 = 0x00,
  IT_s8 = 0x01,
  IT_u16 = 0x02,
  IT_s16 = 0x03,
  IT_u32 = 0x04,
  IT_s32 = 0x05,
  IT_u64 = 0x06,
  IT_s64 = 0x07,
  IT_f32 = 0x08,
  IT_f64 = 0x09,
  IT_string = 0x0a,
  IT_ref = 0x0b,
};
enum ITInstrKind {
  // Lifting
  II_liftInt,
  II_liftFloat,
  II_memToString,
  II_indexToRef,

  // Middle-end?
  II_argGet,
  II_callExport,
  II_callImport,

  // Lowering
  II_lowerInt,
  II_lowerFloat,
  II_stringToMem,
  II_refToIndex,
};
struct ITType {
  ITTypeKind kind;

  void write(llvm::raw_ostream &os) const {
    os.write(kind);
  }
};
struct ITInstr {
  ITInstrKind kind;
  std::vector<unsigned char> immediates;

  void write(llvm::raw_ostream &os) const {
    os.write(kind);
    for (auto imm : immediates) {
      os.write(imm);
    }
  }
};
struct ITFuncDecl {
  std::string name;
  std::vector<ITType> params;
  std::vector<ITType> results;
  std::vector<ITInstr> instrs;

  void write(llvm::raw_ostream &os) const {
    write_str(name, os);
    write_vec(params, os);
    write_vec(results, os);
    write_vec(instrs, os);
  }
};

std::vector<unsigned char> encode_string(std::string str) {
  std::vector<unsigned char> result;
  // TODO: LEB
  result.push_back(str.size());
  for (unsigned i = 0; i < str.size(); ++i) {
    result.push_back(str[i]);
  }
  return result;
}

class MyConsumer : public ASTConsumer,
                   public RecursiveASTVisitor<MyConsumer> {
  typedef RecursiveASTVisitor<MyConsumer> base;
  llvm::raw_ostream *os;
  std::error_code EC;
  std::unique_ptr<ASTNameGenerator> mangler;

  std::vector<ITFuncDecl> funcDecls;

public:
  MyConsumer() {
    if (Outfile != "") {
      os = new llvm::raw_fd_ostream(Outfile, EC);
    } else {
      os = &llvm::outs();
    }
  }
  ~MyConsumer() {
    if (funcDecls.size()) {
      write_vec(funcDecls, *os);
    }
    if (Outfile != "") {
      delete os;
    }
  }
  void HandleTranslationUnit(ASTContext &Context) override {
    auto *D = Context.getTranslationUnitDecl();
    mangler = std::make_unique<ASTNameGenerator>(Context);
    TraverseDecl(D);
  }
  const std::string prefix = "EM_IMPORT:";
  const int prefixLen = prefix.size();

  void doTheThing(Decl* D, std::string className = "") {
    if (auto FD = dyn_cast_or_null<FunctionDecl>(D)) {
      auto name = FD->getQualifiedNameAsString();
      if (auto* Attr = FD->getAttr<AnnotateAttr>()) {
        auto note = Attr->getAnnotation();
        if (note.startswith(prefix)) {
          auto data = StringRef(note.data() + prefixLen, note.size() - prefixLen);
          auto pair = data.split(":");
          auto kind = pair.first;
          auto importName = pair.second;
          auto T = FD->getType()->castAs<FunctionProtoType>();
          ITFuncDecl decl;
          decl.name = mangler->getName(FD);

          // Lift params
          for (unsigned i = 0; i < T->getNumParams(); ++i) {
            auto Arg = T->getParamType(i).getCanonicalType();
            std::string ArgStr = Arg.getAsString();
            if (ArgStr == "const char *") {
              decl.params.push_back({ IT_string });

              // ptr
              decl.instrs.push_back({ II_argGet, {i}}); // TODO: LEB-encode {i}
              // len
              decl.instrs.push_back({ II_argGet, {i}});
              decl.instrs.push_back({ II_callExport, encode_string("em_strlen")});
              // convert
              decl.instrs.push_back({ II_memToString, {}});
            } else if (ArgStr == "int") {
              decl.params.push_back({ IT_s32 });

              decl.instrs.push_back({ II_argGet, {i}});
              decl.instrs.push_back({ II_liftInt, {}});
            } else {
              llvm_unreachable(("Unimplemented param: " + ArgStr).c_str());
            }
          }

          // Do function call
          decl.instrs.push_back({ II_callImport, encode_string(decl.name)});

          // Lift result
          auto Ret = T->getReturnType().getCanonicalType();
          std::string RetStr = Ret.getAsString();
          if (RetStr == "void") {
            // no result
          } else if (RetStr == "struct emscripten::JSObject") {
            decl.results.push_back({ IT_ref });
            decl.instrs.push_back({ II_refToIndex, {}});
          } else {
            llvm_unreachable(("Unimplemented result: " + RetStr).c_str());
          }
          funcDecls.push_back(decl);
        }
      }
    }
  }
  bool TraverseDecl(Decl *D) {
    if (auto *TD = dyn_cast_or_null<RecordDecl>(D)) {
      if (auto *Attr = D->getAttr<AnnotateAttr>()) {
        auto note = Attr->getAnnotation();
        if (note.startswith(prefix)) {
          auto data = StringRef(note.data() + prefixLen, note.size() - prefixLen);
          auto pair = data.split(":");
          auto className = pair.second;
          for (auto *sub : TD->decls()) {
            doTheThing(sub, className.str());
          }
        }
      }
    } else if (auto FD = dyn_cast_or_null<FunctionDecl>(D)) {
      if (auto *Attr = D->getAttr<AnnotateAttr>()) {
        auto note = Attr->getAnnotation();
        if (note.startswith("EM_IMPORT:func:")) {
          doTheThing(D);
        }
      }
    }
    return base::TraverseDecl(D);
  }
};

class ClangCheckActionFactory {
public:
  std::unique_ptr<clang::ASTConsumer> newASTConsumer() {
    return std::make_unique<MyConsumer>();
  }
};

} // namespace

int main(int argc, const char **argv) {
  llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);

  // Initialize targets for clang module support.
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmPrinters();
  llvm::InitializeAllAsmParsers();

  CommonOptionsParser OptionsParser(argc, argv, EmImportCategory);
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  ClangCheckActionFactory CheckFactory;
  std::unique_ptr<FrontendActionFactory> FrontendFactory =
    newFrontendActionFactory(&CheckFactory);

  return Tool.run(FrontendFactory.get());
}
