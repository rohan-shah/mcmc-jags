set(sarray_cpp Range.cc SimpleRange.cc RangeIterator.cc SArray.cc)
add_library(sarray OBJECT ${sarray_cpp})
target_include_directories(sarray PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../util)