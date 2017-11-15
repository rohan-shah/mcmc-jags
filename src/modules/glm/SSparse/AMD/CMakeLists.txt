set(AMDSources Source/amd_1.c Source/amdbar.f Source/amd_dump.c Source/amd_info.c Source/amd_post_tree.c Source/amd_2.c Source/amd_control.c Source/amd.f Source/amd_order.c Source/amd_preprocess.c Source/amd_aat.c Source/amd_defaults.c Source/amd_global.c Source/amd_postorder.c Source/amd_valid.c)
set(AMDHeaders Include/amd.h Include/amd_internal.h)
add_library(AMD STATIC ${AMDSources} ${AMDHeaders})
target_include_directories(AMD PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include ${CMAKE_CURRENT_SOURCE_DIR}/../config)