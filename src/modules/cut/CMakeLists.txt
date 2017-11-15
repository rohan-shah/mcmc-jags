add_library(cut STATIC cut.cc DCut.cc DCut.h)
target_include_directories(cut PUBLIC ${CMAKE_SOURCE_DIR}/src/include .)