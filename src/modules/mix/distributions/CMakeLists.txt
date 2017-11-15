add_library(mixDistributions STATIC DNormMix.cc DBetaBin.cc DNormMix.h DBetaBin.h)
target_include_directories(mixDistributions PRIVATE ${CMAKE_SOURCE_DIR}/src/Include)