add_library(function OBJECT Function.cc ScalarFunction.cc LinkFunction.cc VectorFunction.cc ArrayFunction.cc FunctionPtr.cc FuncTab.cc FuncError.cc DPQFunction.cc DFunction.cc PFunction.cc QFunction.cc ScalarLogDensity.cc VectorLogDensity.cc ArrayLogDensity.cc ScalarVectorFunction.cc)
if(NOT WIN32)
	target_compile_options(function PRIVATE -fPIC)
endif()
