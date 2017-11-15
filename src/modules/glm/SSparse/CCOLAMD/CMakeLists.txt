add_library(CCOLAMD STATIC Source/ccolamd.c Include/ccolamd.h)
target_include_directories(CCOLAMD PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include ${CMAKE_CURRENT_SOURCE_DIR}/../config)