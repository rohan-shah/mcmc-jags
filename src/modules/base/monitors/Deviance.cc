#include "Deviance.h"

#include <graph/StochasticNode.h>

#include <stdexcept>
#include <algorithm>

using std::vector;
using std::logic_error;
using std::string;
using std::copy;

static vector<Node const *> toNode(vector<StochasticNode const *> const &nodes)
{
    vector<Node const *> ans(nodes.size());
    for (unsigned int i = 0; i < nodes.size(); ++i) {
	ans[i] = nodes[i];
    }
    return ans;
}

namespace base {

    Deviance::Deviance(vector<StochasticNode const *> const &
				     snodes)
	: Monitor("trace", toNode(snodes)), _values(snodes[0]->nchain()), 
	  _snodes(snodes)
    {
    }

    unsigned int Deviance::nchain() const
    {
	return _snodes[0]->nchain();
    }

    vector<unsigned int> Deviance::dim() const
    {
	vector<unsigned int> d(2);
	d[0] = _values[0].size();
	d[1] = nchain();
	return d;
    }

    vector<unsigned int> Deviance::dim1() const
    {
	return vector<unsigned int>(1,1);
    }
 
    vector<double> const &Deviance::value(unsigned int chain) const
    {
	return _values[chain];
    }

    void Deviance::update()
    {
	unsigned int nchain = _snodes[0]->nchain();
	for (unsigned int ch = 0; ch < nchain; ++ch) {
	    double loglik = 0;
	    for (unsigned int i = 0; i < _snodes.size(); ++i) {
		loglik += _snodes[i]->logDensity(ch);
	    }
	    _values[ch].push_back(-2 * loglik);
	}
    }

    void Deviance::reserve(unsigned int niter)
    {
	for (unsigned int ch = 0; ch < _values.size(); ++ch) {
	    _values[ch].reserve(_values[ch].size() + niter);
	}
    }
    
    SArray Deviance::dump() const
    {
	unsigned int length = _values[0].size(); 
	unsigned int nchain = _values.size();

	vector<double> v(length * nchain);
	vector<double>::iterator p = v.begin();
	for (unsigned int ch = 0; ch < nchain; ++ch) {
	    p = copy(_values[ch].begin(), _values[ch].end(), p);
	}

	SArray ans(dim());
	ans.setValue(v);    
	
	vector<string> names(2);
	names[0] = "iteration";
	names[1] = "chain";
	ans.setDimNames(names);
	return(ans);
    }

    bool Deviance::poolChains() const
    {
	return false;
    }

    bool Deviance::poolIterations() const
    {
	return false;
    }

}
