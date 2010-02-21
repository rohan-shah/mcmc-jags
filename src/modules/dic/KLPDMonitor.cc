#include "KLPDMonitor.h"

#include <graph/StochasticNode.h>
#include <rng/RNG.h>

using std::vector;

namespace dic {

    KLPDMonitor::KLPDMonitor(vector<StochasticNode const *> const &snodes,
			     vector<KL const *> const &kl)
	: PDMonitor(snodes), _snodes(snodes), _kl(kl)
    {
    }

    double KLPDMonitor::divergence(unsigned int k, unsigned int ch1, 
				   unsigned int ch2) const
    {
	vector<double const*> const &v1 = _snodes[k]->parameters(ch1);
	vector<double const*> const &v2 = _snodes[k]->parameters(ch2);
	return _kl[k]->divergence(v1, v2) + _kl[k]->divergence(v2, v1);
    }


}
