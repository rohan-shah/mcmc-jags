#include <config.h>
#include <graph/ScalarLogicalNode.h>
#include <function/ScalarFunc.h>
#include <graph/NodeError.h>
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
using std::runtime_error;

ScalarLogicalNode::ScalarLogicalNode(ScalarFunc const *function, 
			 vector<Node const *> const &parameters)
    : DeterministicNode(vector<unsigned int>(1,1), parameters),
      _func(function),
      _parameters(nchain())
{
    for (unsigned int j = 0; j < parameters.size(); ++j) {
	if (!isScalar(parameters[j]->dim())) {
	    throw logic_error("Invalid parameter dims in ScalarScalarLogicalNode");
	}
    }

    for (unsigned int n = 0; n < nchain(); ++n) {
	_parameters[n].reserve(parameters.size());
	for (unsigned long j = 0; j < parameters.size(); ++j) {
	    _parameters[n].push_back(parameters[j]->value(n));
	}
	
    }

}

string ScalarLogicalNode::deparse(vector<string> const &parents) const
{
    string name = "(";
    name.append(_func->deparse(parents));
    name.append(")");
	      
    return name;
}

void ScalarLogicalNode::deterministicSample(unsigned int chain)
{
    _data[chain] = _func->evaluateScalar(_parameters[chain]);
}

//FIXME: Identical to LogicalNode::isClosed

bool ScalarLogicalNode::isClosed(set<Node const *> const &ancestors, 
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
    case DNODE_POWER:
	return _func->isPower(mask, fixed_mask);
        break;
    case DNODE_SCALE:
	return _func->isScale(mask, fixed_mask);
	break;
    case DNODE_SCALE_MIX:
        return (nmask == 1) && _func->isScale(mask, fixed_mask);
	break;
    }
}


bool ScalarLogicalNode::checkParentValues(unsigned int chain) const
{
    return _func->checkScalarValue(_parameters[chain]);
}


Node *ScalarLogicalNode::clone(vector<Node const*> const &parents) const
{
    return new ScalarLogicalNode(_func, parents);
}

bool ScalarLogicalNode::isDiscreteValued() const
{
    vector<bool> mask(parents().size());
    for (unsigned long j = 0; j < parents().size(); ++j) {
        mask[j] = parents()[j]->isDiscreteValued();
    }
    return _func->isDiscreteValued(mask);
}

