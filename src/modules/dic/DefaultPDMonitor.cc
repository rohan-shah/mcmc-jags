#include "DefaultPDMonitor.h"

#include <graph/StochasticNode.h>
#include <rng/RNG.h>

using std::vector;

static vector<StochasticNode*> mkRep(vector<StochasticNode const *> const &s)
{
    vector<StochasticNode *> ans(s.size());
    for (unsigned int i = 0; i < s.size(); ++i) {
	vector<Node const *> par(s[i]->parents());
	if (s[i]->upperBound()) {
	    par.pop_back();
	}
	if (s[i]->lowerBound()) {
	    par.pop_back();
	}
	ans[i] = new StochasticNode(s[i]->distribution(), par,
				    s[i]->lowerBound(), s[i]->upperBound());
    }
    return ans;
}

namespace dic {

    DefaultPDMonitor::DefaultPDMonitor(vector<StochasticNode const *> const &s,
				       vector<RNG *> const &rngs, 
				       unsigned int nrep)
	: PDMonitor(s), _repnodes(mkRep(s)), _rngs(rngs), _nrep(nrep)
    {
    }

    DefaultPDMonitor::~DefaultPDMonitor()
    {
	for (unsigned int i = 0; i < _repnodes.size(); ++i) {
	    delete _repnodes[i];
	}
    }

    double DefaultPDMonitor::divergence(unsigned int k, unsigned int i, 
					unsigned int j) const
    {
	StochasticNode *rnode = _repnodes[k];
	unsigned int len = rnode->length();
	
	double div = 0;
	for (unsigned int r = 0; r < _nrep; ++r) {
	    rnode->randomSample(_rngs[i], i);
	    rnode->randomSample(_rngs[j], j);
	    div += rnode->logDensity(i) + rnode->logDensity(j);
	    rnode->swapValue(i,j);
	    div -= rnode->logDensity(i) + rnode->logDensity(j);
	}
	return div / _nrep;
    }

    double DefaultPDMonitor::weight(unsigned int k, unsigned int ch) const
    {
	return 1;
    }

}
