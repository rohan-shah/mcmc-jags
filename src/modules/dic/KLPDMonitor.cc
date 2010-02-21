#include "KLPDMonitor.h"

#include <graph/StochasticNode.h>
#include <rng/RNG.h>

using std::vector;

namespace dic {

    KLPDMonitor::KLPDMonitor(vector<StochasticNode const *> const &snodes,
			     KL const *kl)
	: PDMonitor(snodes), _snodes(snodes), _kl(kl)
    {
    }

    KLPDMonitor::~KLPDMonitor()
    {
        delete _kl;
    }

    double KLPDMonitor::divergence(unsigned int k, unsigned int i, 
				   unsigned int j) const
    {
	vector<double const*> const &vi = _snodes[k]->parameters(i);
	vector<double const*> const &vj = _snodes[k]->parameters(j);
	return _kl->divergence(vi, vj) + _kl->divergence(vj, vi);
    }

}
