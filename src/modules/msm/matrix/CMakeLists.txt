add_library(msmMatrix OBJECT matexp.cc matexp.h lapack.h)
target_include_directories(msmMatrix PRIVATE ${CMAKE_SOURCE_DIR}/src/include ${BLAS_INCLUDE_DIR})
if(NOT WIN32)
	target_compile_options(msmMatrix PRIVATE -fPIC)
endif()