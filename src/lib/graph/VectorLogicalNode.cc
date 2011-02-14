#include <config.h>
#include <graph/VectorLogicalNode.h>
#include <function/FuncError.h>
#include <function/VectorFunction.h>
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

static unsigned int valueLength(VectorFunction const *func, 
				vector<Node const *> const &parents)
{
    /* 
       Calculates length of vector logical node as a function of its
       parameters.
    */
    
    vector<unsigned int> parameter_lengths(parents.size());
    for (unsigned int j = 0; j < parents.size(); ++j) {
	parameter_lengths[j] = parents[j]->length();
    }
    
    if (!func) {
	throw logic_error("NULL function in VectorLogicalNode constructor");
    }
    if (!func->checkNPar(parents.size())) {
	throw FuncError(func, "Incorrect number of parameters");
    }
    if (!func->checkParameterLength(parameter_lengths)) {
	throw FuncError(func, "Non-conforming parameters");
    }
    return func->length(parameter_lengths);
}

static vector<unsigned int> const &
parameterLengths(vector<Node const *> const &parameters) {
    vector<unsigned int> lengths(parameters.size());
    for (unsigned int j = 0; j < parameters.size(); ++j) {
        lengths[j] = parameters[j]->length();
	
    }
    return getUnique(lengths);
}

VectorLogicalNode::VectorLogicalNode(VectorFunction const *function, 
				     vector<Node const *> const &parameters)
    : LogicalNode(vector<unsigned int>(1,valueLength(function, parameters)), 
		  parameters, function),
      _func(function), _lengths(parameterLengths(parameters))
{
    if (isObserved()) {
	for (unsigned int ch = 0; ch < _nchain; ++ch) {
	    deterministicSample(ch);
	}
    }
}

void VectorLogicalNode::deterministicSample(unsigned int chain)
{
    _func->evaluate(_data + chain * _length, _parameters[chain], _lengths);
}

bool VectorLogicalNode::checkParentValues(unsigned int chain) const
{
    return _func->checkParameterValue(_parameters[chain], _lengths);
}

DeterministicNode *
VectorLogicalNode::clone(vector<Node const*> const &parents) const
{
    return new VectorLogicalNode(_func, parents);
}
