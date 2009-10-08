#include <config.h>
#include "GLMSampler.h"
#include <sampler/Updater.h>

using std::vector;

namespace glm {

    GLMSampler::GLMSampler(Updater *updater, 
			   vector<Updater*> const &sub_updaters,
			   vector<SampleMethod*> const &methods)
	: ParallelSampler(updater, methods), _sub_updaters(sub_updaters)
    {
    }
    
    GLMSampler::~GLMSampler()
    {
	while (!_sub_updaters.empty()) {
	    delete _sub_updaters.back();
	    _sub_updaters.pop_back();
	}
    }

}
