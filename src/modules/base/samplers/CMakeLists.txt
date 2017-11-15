set(baseSamplersSources DiscreteSlicer.cc FiniteFactory.cc FiniteMethod.cc RealSlicer.cc SliceFactory.cc)
set(baseSamplersHeaders DiscreteSlicer.h FiniteFactory.h FiniteMethod.h RealSlicer.h SliceFactory.h)
add_library(baseSamplers OBJECT ${baseSamplersSources} ${baseSamplersHeaders})