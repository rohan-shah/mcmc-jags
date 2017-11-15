add_library(COLAMD Source/colamd.c Source/colamd_global.c)
target_include_directories(COLAMD PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include ${CMAKE_CURRENT_SOURCE_DIR}/../config)