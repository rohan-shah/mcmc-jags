#include <config.h>

#include "CatDirichlet.h"
#include "DirichletCat.h"

#include <rng/RNG.h>
#include <graph/MixtureNode.h>
#include <graph/StochasticNode.h>
#include <sampler/GraphView.h>
#include <module/ModuleError.h>

#include <set>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

#include <JRmath.h>

using std::vector;
using std::set;
using std::string;
using std::copy;
using std::map;
using std::pair;


namespace jags {

    namespace mix {
	
	CatDirichlet::CatDirichlet(GraphView const *gv, DirichletCat *dc,
				   unsigned int chain)
	    : _gv(gv), _dc(dc), _chain(chain), _size(gv->nodes()[0]->length())
	{
	}

	void CatDirichlet::update(RNG *rng)
	{
	    vector<StochasticNode*> const &snodes = _gv->nodes();
	    vector<double> xnew(_gv->length());

	    for (unsigned int i = 0; i < snodes.size(); ++i) {

		int index = static_cast<int>(*snodes[i]->value(_chain)) - 1;
		vector<double> &alpha = _dc->getActiveParameter(i);
		alpha[index] -= 1;

		double sump = 0.0;
		unsigned int j = 0;
		for ( ; j < _size; ++j) {
		    sump += alpha[j];
		}
		double p = sump * rng->uniform();    
		for ( ; j > 1; --j) {
		    sump -= alpha[j-1];
		    if (sump <= p) 
			break;
		}
		xnew[i]  = j;
		alpha[j] += 1;
	    }
	}
    }
}

