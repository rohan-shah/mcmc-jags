#include <config.h>

#include "ConjugateLM.h"
#include "GLMSampler.h"
#include <graph/StochasticNode.h>

using std::vector;
using std::string;

namespace glm {

    double ConjugateLM::getMean(vector<StochasticNode const*> const &children, 
				unsigned int i, unsigned int chain) const
    {
	return children[i]->parents()[0]->value(chain)[0];
    }
    
    double 
    ConjugateLM::getPrecision(vector<StochasticNode const*> const &children, 
			      unsigned int i, unsigned int chain) const 
    {
	return children[i]->parents()[1]->value(chain)[0];
    }

    double ConjugateLM::getValue(vector<StochasticNode const *> const &children,
				 unsigned int i, unsigned int chain) const 
    {
	return children[i]->value(chain)[0];
    }
    
    string ConjugateLM::name() const
    {
	return "ConjugateLM";
    }
    
}
