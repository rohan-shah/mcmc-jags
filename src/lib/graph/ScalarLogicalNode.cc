#include <config.h>
#include <graph/ScalarLogicalNode.h>
#include <function/FuncError.h>
#include <function/ScalarFunction.h>
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

ScalarLogicalNode::ScalarLogicalNode(ScalarFunction const *function, 
				     vector<Node const *> const &parameters)
    : LogicalNode(vector<unsigned int>(1,1), parameters, function),
      _func(function)
{
    if (!function) {
	throw logic_error("NULL function in ScalarLogicalNode constructor");
    }
    for (unsigned int j = 0; j < parameters.size(); ++j) {
	if (!isScalar(parameters[j]->dim())) {
	    throw FuncError(function, "Invalid parameter dims");
	}
    }

    if (isObserved()) {
	for (unsigned int ch = 0; ch < _nchain; ++ch) {
	    deterministicSample(ch);
	}
    }
}

void ScalarLogicalNode::deterministicSample(unsigned int chain)
{
    _data[chain] = _func->evaluate(_parameters[chain]);
}

bool ScalarLogicalNode::checkParentValues(unsigned int chain) const
{
    return _func->checkParameterValue(_parameters[chain]);
}

DeterministicNode *
ScalarLogicalNode::clone(vector<Node const*> const &parents) const
{
    return new ScalarLogicalNode(_func, parents);
}


