#include <config.h>

#include "ConjugateDirichlet.h"

#include <rng/RNG.h>
#include <graph/MixtureNode.h>
#include <graph/StochasticNode.h>
#include <graph/AggNode.h>
#include <graph/NodeError.h>
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

    static vector<MixtureNode const *>  mkMixParents(GraphView const *gv) 
    {
	vector<StochasticNode const *> const &schildren =
	    gv->stochasticChildren();
	
	vector<MixtureNode const *> mixnodes;
	mixnodes.reserve(schildren.size());
	for (unsigned int i = 0; i < schildren.size(); ++i) {
	    mixnodes.push_back(asMixture(schildren[i]));
	}
	return mixnodes;
    }

    static map<Node const *, vector<double> > mkParMap(GraphView const *gv)
    {
	vector<StochasticNode *> const &snodes =  gv->nodes();
	map<Node const *, vector<double> > parmap;
	unsigned int N = snodes[0]->length();
	for (unsigned int i = 0; i < snodes.size(); ++i) {
	    if (snodes[i]->length() != N) {
		throwLogicError("Length mismatch in MixDirich");
	    }
	    parmap[snodes[i]] = vector<double>(N);
	}
	return parmap;
    }

    namespace Mix {
	
	bool 
	ConjugateDirichlet::canSample(GraphView const *gv)
	{
	    // Check sample nodes
	    vector<StochasticNode*> snodes = gv->nodes();
	    for (unsigned int i = 0; i < snodes.size(); ++i) {
		if (isBounded(snodes[i])) return false;
		if (snodes[i]->distribution()->name() != "ddirch")
		    return false;
	    }

	    set<StochasticNode*> sset;
	    set<Node*> dset;
	    for (unsigned int i = 0; i < snodes.size(); ++i) {
		sset.insert(snodes[i]);
		dset.insert(snodes[i]);
	    }

	    vector<DeterministicNode*> const &dchildren = 
		gv->deterministicChildren();
	    
	    for (unsigned int i = 0; i < dchildren.size(); ++i) {
		if (MixtureNode const *m = asMixture(dchildren[i])) {
		    vector<Node const*> const &mparents = m->parents();
		    unsigned int Nindex = m->index_size();
		    for (unsigned int j = 0; j < Nindex; ++j) {
			if (dset.count(mparents[j])) return false;
		    }
		    for (unsigned int j = Nindex; j < mparents.size(); ++j) {
			if (sset.count(mparents[j]) == 0) return false;
		    }
		}
		else {
		    return false;
		}
		dset.insert(dchildren[i]);
	    }
    
	    // Check stochastic children
	    vector<StochasticNode const*> const &schildren = 
		gv->stochasticChildren();
	    
	    for (unsigned int i = 0; i < schildren.size(); ++i) {
		if (isBounded(schildren[i])) 
		    return false;
		if (schildren[i]->distribution()->name() != "dcat")
		    return false;
		if (sset.count(schildren[i]->parents()[0]))
		    return false;
	    }

	    return true;
	}
	
	ConjugateDirichlet::ConjugateDirichlet(GraphView const *gv)
	    : _mixparents(mkMixParents(gv)), _parmap(mkParMap(gv))
	{
	}

	void ConjugateDirichlet::update(unsigned int chain, RNG *rng) const
	{
	    StochasticNode *snode = _gv->nodes()[0];
	    unsigned int size = snode->length();
	    double *alpha = new double[size];
	    double *xnew = new double[size];

	    double const *prior = snode->parents()[0]->value(chain);
	    for (unsigned long i = 0; i < size; ++i) {
		alpha[i] = prior[i];
	    }

	    vector<StochasticNode const*> const &schild = _gv->stochasticChildren();
	    for (unsigned int i = 0; i < schild.size(); ++i) {
		unsigned int index = 0;
		double const *N = 0;

		if (isActiveLeaf(i, chain)) {
		    switch(_child_dist[i]) {
		    case MULTI:
			N = schild[i]->value(chain);
			if (_offsets[i].empty()) {
			    for (unsigned int j = 0; j < size; ++j) {
				alpha[j] += N[j];
			    }
			}
			else {
			    for (unsigned int j = 0; j < size; ++j) {
				alpha[j] += N[_offsets[i][j]];
			    }
			}
			break;
		    case CAT:
			index = static_cast<unsigned int>(*schild[i]->value(chain)) - 1;
			if (_offsets[i].empty()) {
			    alpha[index] += 1;
			}
			else {
			    for (unsigned int j = 0; j < size; ++j) {
				if (index == _offsets[i][j]) {
				    alpha[j] += 1;
				    break;
				}
			    }
			}
			break;
		    default:
			throwLogicError("Invalid distribution in ConjugateDirichlet");
		    }
		}
	    }

	    /* Check structural zeros */
	    for (unsigned int i = 0; i < size; ++i) {
		if (prior[i] == 0 && alpha[i] != 0) {
		    throwNodeError(snode, "Invalid likelihood for Dirichlet distribution with structural zeros");
		}
	    }
  
	    /* 
	       Draw Dirichlet sample by drawing independent gamma random
	       variates and then normalizing
	    */

	    double xsum = 0.0;
	    for (unsigned long i = 0; i < size; ++i) {
		if (alpha[i] > 0) {
		    xnew[i] = rgamma(alpha[i], 1, rng);
		    xsum += xnew[i];
		}
		else {
		    xnew[i] = 0;
		}
	    }
	    for (unsigned long i = 0; i < size; ++i) {
		xnew[i] /= xsum;
	    }

	    if (_mix) {

		//Override default updating of deterministic nodes in gv

		snode->setValue(xnew, size, chain);

		vector<DeterministicNode*> const &dchild = _gv->deterministicChildren();
		vector<bool> modified(dchild.size(), false);
		for (unsigned int d = 0; d < dchild.size(); ++d) {
		    if (_tree[d] == -1) {
			MixtureNode *m = dynamic_cast<MixtureNode*>(dchild[d]);
			if (m == 0 || m->activeParent(chain) == snode) {
			    dchild[d]->deterministicSample(chain);
			    modified[d] = true;
			}
		    }
		    else if (modified[_tree[d]]) {
			MixtureNode *m = dynamic_cast<MixtureNode*>(dchild[d]);
			if (m == 0 || m->activeParent(chain) == dchild[_tree[d]]) {
			    dchild[d]->deterministicSample(chain);
			    modified[d] = true;
			}
		    }
		}
	    }
	    else {
		_gv->setValue(xnew, size, chain);
	    }

	    delete [] xnew;
	    delete [] alpha;
	}


	string ConjugateDirichlet::name() const
	{
	    return "ConjugateDirichlet";
	}

    } //namespace jags
} //namespace bugs

