add_library(module OBJECT Module.cc ModuleError.cc)
target_compile_definitions(module PRIVATE MAKE_DLL)