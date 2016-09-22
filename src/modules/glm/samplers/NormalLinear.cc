#include <config.h>

#include "NormalLinear.h"
#include "Classify.h"

#include <graph/StochasticNode.h>

namespace jags {
namespace glm {

    NormalLinear::NormalLinear(StochasticNode const *snode, unsigned int chain)
	: Outcome(snode, chain),
	  _value(snode->value(chain)[0]),
	  _precision(snode->parents()[1]->value(chain)[0])
    {
    }
    
    double NormalLinear::value() const 
    {
	return _value;
    }

    double NormalLinear::precision() const 
    {
	return _precision;
    }

    bool NormalLinear::canRepresent(StochasticNode const *snode)
    {
	return getFamily(snode) == GLM_NORMAL && getLink(snode) == LNK_LINEAR;
    }

    bool NormalLinear::fixedb() const
    {
	return true;
    }

    bool NormalLinear::fixedA() const
    {
	return true;
    }
    

}}
