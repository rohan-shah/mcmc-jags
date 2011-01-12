#include <config.h>
#include "PoptMonitor.h"

#include <graph/StochasticNode.h>

#include <cmath>

using std::vector;
using std::exp;

namespace dic {

    PoptMonitor::PoptMonitor(vector<StochasticNode const *> const &snodes,
			     vector<CalKL *> const &calkl)
	: PDMonitor(snodes, calkl, 2.0), _snodes(snodes)
    {
    }

    double PoptMonitor::weight(unsigned int k, unsigned int ch) const
    {
	return exp(-_snodes[k]->logDensity(ch, PDF_FULL));
    }
}
