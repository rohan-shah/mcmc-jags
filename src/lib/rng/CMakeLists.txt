add_library(rng OBJECT RNG.cc RmathRNG.cc TruncatedNormal.cc)
if(NOT WIN23)
	target_compile_options(rng PRIVATE -fPIC)
endif()
