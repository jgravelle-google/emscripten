# set( LLVM_LINK_COMPONENTS
#   ${LLVM_TARGETS_TO_BUILD}
#   Option
#   Support
# )

add_executable(em-import
  EmImport.cpp
)

# TODO: get these from llvm-config I guess
    # llvm_config(LLVM_CXXFLAGS "--cxxflags")
    # llvm_config(LLVM_LDFLAGS "--ldflags")
    # llvm_config(LLVM_LIBS "--libs")
    # llvm_config(LLVM_LIBDIR "--libdir")
    # llvm_config(LLVM_INCLUDE_DIR "--includedir")
    # llvm_config(LLVM_SYSTEM_LIBS "--system-libs")
set(LLVM_CXXFLAGS "-I/usr/local/google/home/jgravelle/code/wasm/waterfall/src/work/llvm-project/llvm/include -I/usr/local/google/home/jgravelle/code/wasm/waterfall/src/work/llvm-project/clang/include -I/usr/local/google/home/jgravelle/code/wasm/waterfall/src/work/llvm-out/include -I/usr/local/google/home/jgravelle/code/wasm/waterfall/src/work/llvm-out/tools/clang/include -std=c++14   -fno-exceptions -fno-rtti -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS")
set(LLVM_LDFLAGS "-L/usr/local/google/home/jgravelle/code/wasm/waterfall/src/work/llvm-out/lib")
set(LLVM_LIBS "-lLLVM-10git")
set(LLVM_LIBDIR "/usr/local/google/home/jgravelle/code/wasm/waterfall/src/work/llvm-out/lib")
set(LLVM_INCLUDE_DIR "/usr/local/google/home/jgravelle/code/wasm/waterfall/src/work/llvm-project/llvm/include")
set(CLANG_INCLUDE_DIR "/usr/local/google/home/jgravelle/code/wasm/waterfall/src/work/llvm-project/clang/include")

add_definitions(${LLVM_CXXFLAGS})
add_definitions(-fno-rtti)
include_directories(${LLVM_INCLUDE_DIR})
include_directories(${CLANG_INCLUDE_DIR})
link_directories(${LLVM_LIBDIR})

target_link_libraries(em-import
  ${LLVM_LDFLAGS}
  -Wl,--start-group
  -lclang
  -lclangAnalysis
  -lclangAST
  -lclangBasic
  -lclangDriver
  -lclangEdit
  -lclangFrontend
  -lclangParse
  -lclangRewriteFrontend
  -lclangSerialization
  -lclangStaticAnalyzerFrontend
  -lclangTooling
  -lclangSema
  -lclangLex
  -Wl,--end-group
  ${LLVM_LIBS}
)
