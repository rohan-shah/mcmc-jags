#include <config.h>
#include <graph/ArrayLogicalNode.h>
#include <function/FuncError.h>
#include <function/ArrayFunction.h>
#include <util/dim.h>

#include <stdexcept>
#include <vector>
#include <string>

using std::vector;
using std::string;
using std::logic_error;

static vector<unsigned int> mkDim(ArrayFunction const *func, 
				  vector<Node const *> const &parents)
{
    /* 
       Calculates dimension of logical node as a function of its
       parameters.
    */

    vector<vector<unsigned int> > parameter_dims(parents.size());
    for (unsigned int j = 0; j < parents.size(); ++j) {
	parameter_dims[j] = parents[j]->dim();
    }
    if (!func) {
	throw logic_error("NULL function in ArrayLogicalNode constructor");
    }
    if (!func->checkNPar(parameter_dims.size())) {
	throw FuncError(func, "Incorrect number of parameters");
    }
    if (!func->checkParameterDim(parameter_dims)) {
	throw FuncError(func, "Non-conforming parameters");
    }
    return func->dim(parameter_dims);
}

static vector<vector<unsigned int> > const &
mkParameterDims(vector<Node const *> const &parameters) {
    vector<vector<unsigned int> > dims(parameters.size());
    for (unsigned int j = 0; j < parameters.size(); ++j) {
        dims[j] = parameters[j]->dim();
    }
    return getUnique(dims);
}

ArrayLogicalNode::ArrayLogicalNode(ArrayFunction const *function, 
				   vector<Node const *> const &parameters)
    : LogicalNode(mkDim(function,parameters), parameters, function),
      _func(function), _dims(mkParameterDims(parameters))
{
    if (isObserved()) {
	for (unsigned int ch = 0; ch < _nchain; ++ch) {
	    deterministicSample(ch);
	}
    }
}

void ArrayLogicalNode::deterministicSample(unsigned int chain)
{
    _func->evaluate(_data + chain * _length, _parameters[chain], _dims);
}

bool ArrayLogicalNode::checkParentValues(unsigned int chain) const
{
    return _func->checkParameterValue(_parameters[chain], _dims);
}

DeterministicNode *
ArrayLogicalNode::clone(vector<Node const*> const &parents) const
{
    return new ArrayLogicalNode(_func, parents);
}

