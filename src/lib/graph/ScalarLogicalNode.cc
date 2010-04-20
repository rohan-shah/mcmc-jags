#include <config.h>
#include <graph/ScalarLogicalNode.h>
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
using std::runtime_error;

ScalarLogicalNode::ScalarLogicalNode(ScalarFunction const *function, 
				     vector<Node const *> const &parameters)
    : LogicalNode(vector<unsigned int>(1,1), parameters, function),
      _func(function)
{
    for (unsigned int j = 0; j < parameters.size(); ++j) {
	if (!isScalar(parameters[j]->dim())) {
	    throw logic_error("Invalid parameter dims in ScalarLogicalNode");
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


