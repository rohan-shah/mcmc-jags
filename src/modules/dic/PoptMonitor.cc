#include <config.h>
#include "PoptMonitor.h"

#include <graph/StochasticNode.h>

#include <cmath>

using std::vector;
using std::exp;

namespace jags {
namespace dic {

    PoptMonitor::PoptMonitor(vector<StochasticNode const *> const &snodes,
			     vector<RNG *> const &rngs, unsigned int nrep)
	: PDMonitor(snodes, rngs, nrep, 2.0)
    {
    }

    double PoptMonitor::weight(StochasticNode const *snode,
			       unsigned int ch) const
    {
	return exp(-snode->logDensity(ch, PDF_FULL));
    }

}}
