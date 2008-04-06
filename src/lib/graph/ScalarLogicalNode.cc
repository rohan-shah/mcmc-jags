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

    vector<bool> mask(parameters.size());
    for (unsigned long j = 0; j < parameters.size(); ++j) {
	mask[j] = parameters[j]->isDiscreteValued();
    }
    if (_func->isDiscreteValued(mask)) {
	setDiscreteValued();
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

bool ScalarLogicalNode::isLinear(GraphMarks const &linear_marks, bool fixed) const
{
    vector<bool> mask(parents().size());
    for (unsigned int i = 0; i < parents().size(); ++i) {
	Node const *p = parents()[i];
        if (linear_marks.graph().contains(p)) {
	    switch(linear_marks.mark(p)) {
            case MARK_NULL:
                mask[i] = false;
                break;
	    case MARK_TRUE:
		mask[i] = true;
		break;
	    case MARK_FALSE:
                //Parent is a non-linear function. No way to recover.
		return false;
		break;
	    default:
		throw logic_error("Invalid marks in ScalarlogicalNode::isLinear");
	    }
	}
        else {
            //We don't care if the function is non-linear in these parameters
            mask[i] = false; 
        }
    }

    vector<bool> fixed_mask;
    if (fixed) {
	for (unsigned int i = 0; i < parents().size(); ++i) {
	    fixed_mask.push_back(parents()[i]->isObserved());
	}
    }

    return _func->isLinear(mask, fixed_mask);
}

bool ScalarLogicalNode::isScale(GraphMarks const &scale_marks, bool fixed) const
{
    unsigned index = 0;
    bool have_index = false;
    vector<Node const *> const &par = parents();
    for (unsigned int i = 0; i < par.size(); ++i) {
	if (scale_marks.graph().contains(par[i])) {
	    switch(scale_marks.mark(par[i])) {
            case MARK_NULL:
		break;
	    case MARK_TRUE:
		if (!have_index) {
		    have_index = true;
		    index = i;
		}
		else {
		    return false;
		}
		break;
	    case MARK_FALSE:
		return false;
		break;
	    default:
		throw logic_error("Invalid marks in ScalarLogicalNode::isScale");
	    }
	}
    }
    if (!have_index) {
	return true;
    }
    
    vector<bool> fixed_mask;
    if (fixed) {
	for (unsigned int i = 0; i < par.size(); ++i) {
	    fixed_mask.push_back(par[i]->isObserved());
	}
    }

    return _func->isScale(index, fixed_mask);
}

bool ScalarLogicalNode::checkParentValues(unsigned int chain) const
{
    return _func->checkScalarValue(_parameters[chain]);
}


Node *ScalarLogicalNode::clone(vector<Node const*> const &parents) const
{
    return new ScalarLogicalNode(_func, parents);
}
