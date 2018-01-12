add_library(sampler OBJECT Sampler.cc GraphView.cc Slicer.cc Metropolis.cc RWMetropolis.cc MutableSampleMethod.cc ImmutableSampleMethod.cc Linear.cc SamplerFactory.cc SingletonFactory.cc StepAdapter.cc TemperedMetropolis.cc MutableSampler.cc ImmutableSampler.cc)
if(NOT WIN32)
	target_compile_options(sampler PRIVATE -fPIC)
endif()
