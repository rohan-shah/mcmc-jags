#include "CalKLExact.h"

#include "KL.h"
#include <graph/StochasticNode.h>

namespace dic {

    CalKLExact::CalKLExact(StochasticNode const *snode, KL const *kl)
	: _snode(snode), _kl(kl)
    {}

    double CalKLExact::divergence(unsigned int i, unsigned int j) const
    {
	return _kl->divergence(_snode->parameters(i), _snode->parameters(j)) +
	    _kl->divergence(_snode->parameters(j), _snode->parameters(i));
    }

}

    
