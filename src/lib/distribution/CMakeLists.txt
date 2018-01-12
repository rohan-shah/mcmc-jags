add_library(distribution OBJECT Distribution.cc ScalarDist.cc VectorDist.cc ArrayDist.cc DistPtr.cc DistTab.cc DistError.cc RScalarDist.cc c99_math.c)
if(NOT WIN32)
	target_compile_options(distribution PRIVATE -fPIC)
endif()
