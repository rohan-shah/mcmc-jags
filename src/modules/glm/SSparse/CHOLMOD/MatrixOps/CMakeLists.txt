set(matrixOpsSources cholmod_drop.c cholmod_scale.c	cholmod_submatrix.c cholmod_horzcat.c cholmod_sdmult.c cholmod_symmetry.c cholmod_norm.c cholmod_ssmult.c cholmod_vertcat.c)

add_library(matrixOps ${matrixOpsSources})
target_include_directories(matrixOps PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Include ${CMAKE_CURRENT_SOURCE_DIR}/../../config)