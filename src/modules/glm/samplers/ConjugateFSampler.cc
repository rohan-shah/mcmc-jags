#include <config.h>
#include "ConjugateFSampler.h"
#include "ConjugateFMethod.h"

using std::string;
using std::vector;

namespace glm {
    
    ConjugateFSampler::ConjugateFSampler(GraphView *gv1, GraphView *gv2, 
					 vector<ConjugateFMethod*> const &methods)
	: Sampler(gv1), _gv2(gv2), _methods(methods)
    {}
    
    ConjugateFSampler::~ConjugateFSampler()
    {
	delete _gv2;
    }

    bool ConjugateFSampler::isAdaptive() const
    {
	return false;
    }

    void ConjugateFSampler::adaptOff()
    {
    }

    bool ConjugateFSampler::checkAdaptation() const
    {
	return true;
    }
    string ConjugateFSampler::name() const
    {
	return "conjugateF";
    }

    void ConjugateFSampler::update(vector<RNG*> const &rngs)
    {
	for (unsigned int i = 0; i < _methods.size(); ++i) {
	    _methods[i]->update(rngs[i]);
	}
    }

}
