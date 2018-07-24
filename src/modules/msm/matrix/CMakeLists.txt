add_library(msmMatrix matexp.cc matexp.h lapack.h)
target_include_directories(msmMatrix PRIVATE ${CMAKE_SOURCE_DIR}/src/include ${BLAS_INCLUDE_DIR})
target_link_libraries(msmMatrix PUBLIC ${BLAS_LIBRARIES})
