set(CHOLMODCoreSources cholmod_aat.c cholmod_copy.c	cholmod_transpose.c cholmod_add.c cholmod_dense.c cholmod_triplet.c	cholmod_band.c cholmod_error.c cholmod_change_factor.c cholmod_factor.c cholmod_common.c cholmod_memory.c cholmod_complex.c	cholmod_sparse.c)

add_library(CHOLMODCore STATIC ${CHOLMODCoreSources})
target_include_directories(CHOLMODCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Include ${CMAKE_CURRENT_SOURCE_DIR}/../../config)