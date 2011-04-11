#include <config.h>

#include "PDMonitor.h"
#include "CalKL.h"
#include <graph/StochasticNode.h>
#include <module/ModuleError.h>

#include <algorithm>

using std::vector;
using std::string;
using std::copy;

static vector<Node const *> toNodeVec(vector<StochasticNode const *> const &s)
{
    vector<Node const *> ans(s.size());
    copy (s.begin(), s.end(), ans.begin());
    return ans;
}

namespace dic {

    PDMonitor::PDMonitor(vector<StochasticNode const *> const &snodes,
			 vector<CalKL *> const &calkl, double scale)
	: Monitor("mean", toNodeVec(snodes)), _calkl(calkl),
	  _values(snodes.size(), 0),  _weights(snodes.size(), 0),
	  _scale(scale), _nchain(snodes[0]->nchain())
    {
	if (snodes[0]->nchain() < 2) {
	    throwLogicError("PDMonitor needs at least 2 chains");
	}
    }

    PDMonitor::~PDMonitor() 
    {
	for (unsigned int i = 0; i < _calkl.size(); ++i) {
	    delete _calkl[i];
	}
    }

    vector<unsigned int> PDMonitor::dim() const
    {
	return vector<unsigned int> (1,_values.size());
    }
 
    vector<double> const &PDMonitor::value(unsigned int chain) const
    {
	return _values;
    }

    void PDMonitor::reserve(unsigned int niter)
    {
    }

    bool PDMonitor::poolChains() const
    {
	return true;
    }

    bool PDMonitor::poolIterations() const
    {
	return true;
    }

    void PDMonitor::update()
    {
	vector<double> w(_nchain);
	for (unsigned int k = 0; k < _values.size(); ++k) {
	    
	    double pdsum = 0;
	    double wsum = 0;
	    for (unsigned int i = 0; i < _nchain; ++i) {
		w[i] = weight(k, i);
		for (unsigned int j = 0; j < i; ++j) {
		    pdsum += w[i] * w[j] * _calkl[k]->divergence(i, j);
		    wsum += w[i] * w[j];
		}
	    }
	    pdsum /= wsum;
	    pdsum *= _scale/2;

	    _weights[k] += wsum;
	    _values[k] += wsum * (pdsum - _values[k])/_weights[k];
	}
    }


    double PDMonitor::weight(unsigned int k, unsigned int ch) const
    {
	return 1;
    }

}
