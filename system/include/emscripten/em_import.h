#pragma once

namespace emscripten {

struct JSObject {
private:
  // PIMPL pattern; no actual Impl class exists, but this allocates a pointer
  // that we can use for nefarious purposes.
  struct Impl; Impl* impl;
  void setImplToStr(const char*);

public:
  JSObject() = default;
  JSObject(const char* str) {
    setImplToStr(str);
  }
};

#define EM_IMPORT_FUNC(name) \
  __attribute__((annotate("EM_IMPORT:func:" name)))

// EM_IMPORT_STRUCT: declares an external type and methods defined on it
#define EM_IMPORT_STRUCT(importName, cppName, body) \
  struct __attribute__((annotate("EM_IMPORT:struct:" importName))) \
  cppName : public JSObject body;

#define EM_IMPORT_CONSTRUCTOR \
  __attribute__((annotate("EM_IMPORT:constructor")))

#define EM_IMPORT_METHOD(name) \
  __attribute__((annotate("EM_IMPORT:method:" name)))

#define EM_IMPORT_FIELD_GETTER(importName, ty, name) \
  __attribute__((annotate("EM_IMPORT:get:" importName))) \
  ty get_ ## name();
#define EM_IMPORT_FIELD_SETTER(importName, ty, name) \
  __attribute__((annotate("EM_IMPORT:set:" importName))) \
  void set_ ## name(ty);
#define EM_IMPORT_FIELD(importName, ty, name) \
  EM_IMPORT_FIELD_GETTER(importName, ty, name); \
  EM_IMPORT_FIELD_SETTER(importName, ty, name);

EM_IMPORT_FUNC("jsGetGlobal") JSObject getGlobal(const char*);

template <typename T> T upcast(JSObject &&obj) {
  return *(T*)(void*)&obj;
}
template <typename T> T getGlobal(char* name) {
  return upcast<T>(getGlobal(name));
}

extern "C" {
  unsigned strlen(const char*);
  __attribute__((used)) unsigned em_strlen(const char* str) {
    return strlen(str);
  }
}

} // namespace emscripten
