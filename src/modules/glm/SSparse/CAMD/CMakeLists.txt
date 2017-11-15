set(CAMDSources Source/camd_1.c Source/camd_2.c Source/camd_aat.c Source/camd_control.c Source/camd_defaults.c Source/camd_dump.c Source/camd_global.c Source/camd_info.c Source/camd_order.c Source/camd_postorder.c Source/camd_preprocess.c Source/camd_valid.c)
set(CAMDHeaders Include/camd.h Include/camd_internal.h)
add_library(CAMD STATIC ${CAMDSources} ${CAMDHeaders})
target_include_directories(CAMD PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include ${CMAKE_CURRENT_SOURCE_DIR}/../config)