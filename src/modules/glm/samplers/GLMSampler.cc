#include <config.h>

#include <graph/StochasticNode.h>
#include <graph/LogicalNode.h>
#include <distribution/Distribution.h>
#include <function/Function.h>

//#include <map>
#include <string>

#include "GLMSampler.h"
#include "GLMMethod.h"

//using std::map;
using std::string;
using std::vector;
//using std::string;

namespace glm {

    GLMSampler::GLMSampler(vector<StochasticNode *> const &sample_nodes, 
			   GLMMethod *method, Graph const &graph)
	: Sampler(sample_nodes, graph), _method(method)
    {
	_method->initialize(this, graph);
    }

    GLMSampler::~GLMSampler()
    {
	delete _method;
    }

    void GLMSampler::update(vector<RNG*> const &rngs)
    {
	unsigned int nchain = nodes()[0]->nchain();
	for (unsigned int ch = 0; ch < nchain; ++ch) {
	    _method->update(this, ch, rngs[ch]);
	}
    }

    bool GLMSampler::isAdaptive() const
    {
	return false;
    }

    bool GLMSampler::adaptOff()
    {
	return true;
    }

    string GLMSampler::name() const
    {
	return _method->name();
    }

}
