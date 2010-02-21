#include <config.h>

#include "PDMonitor.h"
#include "CalKL.h"
#include <graph/StochasticNode.h>

#include <stdexcept>

using std::vector;
using std::logic_error;
using std::string;

static vector<Node const *> toNodeVec(vector<StochasticNode const *> const &s)
{
    vector<Node const *> ans;
    copy (s.begin(), s.end(), ans.begin());
    return ans;
}

namespace dic {

    PDMonitor::PDMonitor(vector<StochasticNode const *> const &snodes,
			 vector<CalKL *> const &calkl)
	: Monitor("pD", toNodeVec(snodes)), _calkl(calkl),
	  _values(snodes.size(), 0),  _weights(snodes.size(), 0),
	  _nchain(snodes[0]->nchain())
    {
	if (snodes[0]->nchain() < 2) {
	    throw logic_error("PDMonitor needs at least 2 chains");
	}
    }

    unsigned int PDMonitor::nchain() const
    {
	return 1;
    }

    vector<unsigned int> PDMonitor::dim() const
    {
	return vector<unsigned int> (1,_values.size());
    }

    vector<unsigned int> PDMonitor::dim1() const
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

    SArray PDMonitor::dump() const
    {
	SArray ans(dim());
	ans.setValue(_values);
	return ans;
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

	    _weights[k] += wsum;
	    _values[k] += wsum * (pdsum - _values[k])/_weights[k];
	}
    }


    double PDMonitor::weight(unsigned int k, unsigned int ch) const
    {
	return 1;
    }

}
