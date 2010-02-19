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
	: PDMonitor(s), _repnodes(mkRep(s)), _rngs(rngs), _nrep(nrep), _n(0)
    
    {
    }

    DefaultPDMonitor::~DefaultPDMonitor()
    {
	for (unsigned int i = 0; i < _repnodes.size(); ++i) {
	    delete _repnodes[i];
	}
    }

    void DefaultPDMonitor::update()
    {
	_n++;
	for (unsigned int k = 0; k < _values.size(); ++k) {

	    StochasticNode *rnode = _repnodes[k];
	    unsigned int nchain = rnode->nchain();
	    unsigned int len = rnode->length();
	    
	    double pdsum = 0;
	    for (unsigned int r = 0; r < _nrep; ++r) {
		for (unsigned int i = 0; i < nchain; ++i) {
		    rnode->randomSample(_rngs[i], i);
		    pdsum += rnode->logDensity(i);

		    double const *v = rnode->value(i);
		    for (unsigned int j = 0; j < nchain; ++j) {
			if (j != i) {
			    rnode->setValue(v, len, j);
			    pdsum -= rnode->logDensity(j)/(nchain - 1);
			}
		    }
		}
	    }
	    _values[k] += (pdsum/(_nrep * nchain) - _values[k])/_n;
	}
    }
}
