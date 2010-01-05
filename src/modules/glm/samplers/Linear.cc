#include <config.h>

#include "Linear.h"
#include <graph/StochasticNode.h>
#include <sampler/GraphView.h>

using std::vector;
using std::string;

namespace glm {

    Linear::Linear(GraphView const *view, 
		   vector<GraphView const *> const &sub_views,
		   unsigned int chain, bool gibbs)
	: GLMMethod(view, sub_views, chain, false), _gibbs(gibbs)
    {
    }

    double Linear::getPrecision(unsigned int i) const 
    {
	return _view->stochasticChildren()[i]->parents()[1]->value(_chain)[0];
    }

    double Linear::getValue(unsigned int i) const 
    {
	return _view->stochasticChildren()[i]->value(_chain)[0];
    }
    
    string Linear::name() const
    {
	if (_gibbs)
	    return "LinearGibbs";
	else
	    return "Linear";
    }

    void Linear::update(RNG *rng)
    {
	if (_gibbs) 
	    updateLMGibbs(rng);
	else
	    updateLM(rng);
    }
}
