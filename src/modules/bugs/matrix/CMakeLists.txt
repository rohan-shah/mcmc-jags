set(bugsMatrixSources matrix.cc)
set(bugsMatrixHeaders lapack.h matrix.h)
add_library(bugsMatrix OBJECT ${bugsMatrixSources} ${bugsMatrixHeaders})
target_include_directories(bugsMatrix PUBLIC ${MKL_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})