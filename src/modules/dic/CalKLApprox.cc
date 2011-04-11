#include <config.h>
#include "CalKLApprox.h"

#include <graph/StochasticNode.h>
#include <rng/RNG.h>
#include <module/ModuleError.h>

using std::vector;

static StochasticNode* mkRep(StochasticNode const *snode)
{
    vector<Node const *> par = snode->parents();
    return snode->clone(par);
}

namespace dic {

    CalKLApprox::CalKLApprox(StochasticNode const * snode,
			     vector<RNG *> const &rngs, 
			     unsigned int nrep)
	: _repnode(mkRep(snode)), _rngs(rngs), _nrep(nrep)
    {
	if (snode->nchain() != rngs.size())
	    throwLogicError("Incorrect number of rngs in CalKLApprox");
    }

    CalKLApprox::~CalKLApprox()
    {
	delete _repnode;
    }

    double CalKLApprox::divergence(unsigned int i, unsigned int j) const
    {
	double div = 0;
	for (unsigned int r = 0; r < _nrep; ++r) {
	    _repnode->randomSample(_rngs[i], i);
	    _repnode->randomSample(_rngs[j], j);
	    div += _repnode->logDensity(i, PDF_FULL) + 
		_repnode->logDensity(j, PDF_FULL);
	    _repnode->swapValue(i,j);
	    div -= _repnode->logDensity(i, PDF_FULL) + 
		_repnode->logDensity(j, PDF_FULL);
	}
	return div / _nrep;
    }

}
