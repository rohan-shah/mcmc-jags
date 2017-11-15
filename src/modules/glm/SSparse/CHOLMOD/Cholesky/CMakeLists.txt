set(CHOLMODCholeskySources cholmod_amd.c cholmod_analyze.c cholmod_colamd.c cholmod_etree.c cholmod_factorize.c	cholmod_postorder.c cholmod_rcond.c cholmod_resymbol.c cholmod_rowcolcounts.c cholmod_rowfac.c cholmod_solve.c cholmod_spsolve.c )

add_library(CHOLMODCholesky STATIC ${CHOLMODCholeskySources})
target_include_directories(CHOLMODCholesky PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../Include ${CMAKE_CURRENT_SOURCE_DIR}/../../AMD/Include ${CMAKE_CURRENT_SOURCE_DIR}/../../COLAMD/Include ${CMAKE_CURRENT_SOURCE_DIR}/../../config)
