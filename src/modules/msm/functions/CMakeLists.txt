add_library(msmFunctions Mexp.cc Mexp.h)
target_include_directories(msmFunctions PRIVATE ${CMAKE_SOURCE_DIR}/src/modules/msm/matrix ${CMAKE_SOURCE_DIR}/src/include)