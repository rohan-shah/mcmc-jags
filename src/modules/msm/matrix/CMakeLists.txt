add_library(msmMatrix OBJECT matexp.cc matexp.h lapack.h)
target_include_directories(msmMatrix PRIVATE ${CMAKE_SOURCE_DIR}/src/include ${BLAS_INCLUDE_DIR})
