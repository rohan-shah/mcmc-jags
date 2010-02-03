#include "KLPDMonitor.h"

#include <graph/StochasticNode.h>
#include <rng/RNG.h>

using std::vector;

namespace dic {

    KLPDMonitor::KLPDMonitor(StochasticNode const *snode,
			     KL const *kl)
	: PDMonitor(snode), _kl(kl)
    {
	unsigned int nchain = snode->nchain();
	_par.reserve(nchain);
	for (unsigned int i = 0; i < nchain; ++i) {
	    _par.push_back(snode->parameters(i));
	}
    }

    KLPDMonitor::~KLPDMonitor()
    {
        delete _kl;
    }

    void KLPDMonitor::update()
    {
	unsigned int nchain = _par.size();
	
	double pdsum = 0;
	for (unsigned int i = 0; i < nchain; ++i) {
	    for (unsigned int j = 0; j < nchain; ++j) {
		if (j != i) {
		    pdsum += _kl->divergence(_par[i], _par[j]);
		}
	    }
	}

	_values.push_back(pdsum/(nchain * (nchain - 1)));
    }
}
