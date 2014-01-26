#include <config.h>
#include "GLMSampler.h"
#include <sampler/GraphView.h>
#include <sampler/SingletonGraphView.h>

using std::vector;

namespace jags {
namespace glm {

    GLMSampler::GLMSampler(GraphView *view, 
			   vector<SingletonGraphView*> const &sub_views,
			   vector<MutableSampleMethod*> const &methods,
			   std::string const &name)
	: MutableSampler(view, methods, name), _sub_views(sub_views)
    {
    }
    
    GLMSampler::~GLMSampler()
    {
	while (!_sub_views.empty()) {
	    delete _sub_views.back();
	    _sub_views.pop_back();
	}
    }

}}
