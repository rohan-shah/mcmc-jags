#include "KLPDMonitor.h"

#include <graph/StochasticNode.h>
#include <rng/RNG.h>

using std::vector;

namespace dic {

    KLPDMonitor::KLPDMonitor(vector<StochasticNode const *> const &snodes,
			     KL const *kl)
	: PDMonitor(snodes), _snodes(snodes), _kl(kl), _n(0)
    {
    }

    KLPDMonitor::~KLPDMonitor()
    {
        delete _kl;
    }

    void KLPDMonitor::update()
    {
	_n++;
	for (unsigned int k = 0; k < _values.size(); ++k) {
	    
	    unsigned int nchain = _snodes[k]->nchain();

	    double pdsum = 0;
	    for (unsigned int i = 0; i < nchain; ++i) {
		for (unsigned int j = 0; j < nchain; ++j) {
		    if (j != i) {
			pdsum += _kl->divergence(_snodes[k]->parameters(i),
						 _snodes[k]->parameters(j));
		    }
		}
	    }
	    pdsum /= (nchain * (nchain - 1));

	    _values[k] += (pdsum - _values[k])/_n;
	}
    }
}
