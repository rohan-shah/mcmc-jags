#include <config.h>

#include "PDTrace.h"
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

    PDTrace::PDTrace(vector<StochasticNode const *> const &snodes,
		     vector<CalKL *> const &calkl)
	: Monitor("trace", toNodeVec(snodes)), _calkl(calkl),
	  _values(0), _nchain(snodes[0]->nchain())
    {
	if (calkl.size() != snodes.size()) {
	    throwLogicError("Length mismatch in PDTrace constructor");
	}
	if (snodes[0]->nchain() < 2) {
	    throwLogicError("PDTrace needs at least 2 chains");
	}
    }

    PDTrace::~PDTrace() 
    {
	for (unsigned int i = 0; i < _calkl.size(); ++i) {
	    delete _calkl[i];
	}
    }

    vector<unsigned int> PDTrace::dim() const
    {
	return vector<unsigned int> (1,1);
    }
 
    vector<double> const &PDTrace::value(unsigned int chain) const
    {
	return _values;
    }

    void PDTrace::reserve(unsigned int niter)
    {
	_values.reserve(_values.size() + niter);
    }

    bool PDTrace::poolChains() const
    {
	return true;
    }

    bool PDTrace::poolIterations() const
    {
	return false;
    }

    void PDTrace::update()
    {
	double pd = 0;
	for (unsigned int k = 0; k < _calkl.size(); ++k) {
	    for (unsigned int i = 0; i < _nchain; ++i) {
		for (unsigned int j = 0; j < i; ++j) {
		    pd += _calkl[k]->divergence(i, j);
		}
	    }
	}
	pd /= _nchain * (_nchain - 1);
	_values.push_back(pd);
    }

}
