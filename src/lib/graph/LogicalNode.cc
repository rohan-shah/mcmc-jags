#include <config.h>
#include <graph/LogicalNode.h>
#include <function/FuncError.h>
#include <function/Function.h>
#include <graph/GraphMarks.h>
#include <graph/Graph.h>
#include <util/dim.h>

#include <stdexcept>
#include <vector>
#include <string>
#include <math.h>

using std::vector;
using std::string;
using std::set;
using std::logic_error;

static vector<vector<double const *> > 
mkParams(vector<Node const*> const &parents, unsigned int nchain)
{
    vector<vector<double const *> > ans(nchain);
    for (unsigned int n = 0; n < nchain; ++n) {
	ans[n].reserve(parents.size());
	for (unsigned long j = 0; j < parents.size(); ++j) {
	    ans[n].push_back(parents[j]->value(n));
	}
	
    }
    return ans;
}

LogicalNode::LogicalNode(vector<unsigned int> const &dim, 
			 vector<Node const *> const &parameters,
			 Function const *function)
    : DeterministicNode(dim, parameters), _func(function), _discrete(false), 
      _parameters(mkParams(parameters, nchain()))
{
    if (!function->checkNPar(parameters.size())) {
	throw FuncError(function, "Incorrect number of parameters");
    }
    vector<bool> mask(parents().size());
    for (unsigned long j = 0; j < parents().size(); ++j) {
        mask[j] = parents()[j]->isDiscreteValued();
    }
    _discrete = _func->isDiscreteValued(mask);
}

string LogicalNode::deparse(vector<string> const &parents) const
{
    string name = "(";
    name.append(_func->deparse(parents));
    name.append(")");
	      
    return name;
}

bool LogicalNode::isClosed(set<Node const *> const &ancestors, 
			   ClosedFuncClass fc, bool fixed) const
{
    vector<Node const *> const &par = parents();

    vector<bool> mask(par.size());
    vector<bool> fixed_mask;
    unsigned int nmask = 0;
    for (unsigned int i = 0; i < par.size(); ++i) {
	mask[i] = ancestors.count(par[i]);
	if (mask[i]) {
	    ++nmask;
	}
	if (fixed) {
	    fixed_mask.push_back(par[i]->isObserved());
	}
    }

    if (nmask == 0) {
        throw logic_error("Invalid mask in LogicalNode::isClosed");
    }

    switch(fc) {
    case DNODE_LINEAR:
	return _func->isLinear(mask, fixed_mask);
        break;
    case DNODE_SCALE:
	return _func->isScale(mask, fixed_mask);
	break;
    case DNODE_SCALE_MIX:
        return (nmask == 1) && _func->isScale(mask, fixed_mask);
	break;
    case DNODE_POWER:
	return _func->isPower(mask, fixed_mask);
        break;
    }

    return false; //Wall
}

bool LogicalNode::isDiscreteValued() const
{
    return _discrete;
}
