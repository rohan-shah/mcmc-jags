#include <config.h>
#include "GLMSampler.h"
#include <sampler/GraphView.h>

using std::vector;

namespace glm {

    GLMSampler::GLMSampler(GraphView *view, 
			   vector<GraphView*> const &sub_views,
			   vector<SampleMethod*> const &methods)
	: ParallelSampler(view, methods), _sub_views(sub_views)
    {
    }
    
    GLMSampler::~GLMSampler()
    {
	while (!_sub_views.empty()) {
	    delete _sub_views.back();
	    _sub_views.pop_back();
	}
    }

}
