#include <config.h>
#include <graph/VSLogicalNode.h>
#include <function/ScalarFunction.h>

#include <stdexcept>
#include <vector>
#include <math.h>

using std::vector;
using std::logic_error;

static unsigned int mkLength(vector<Node const *> const &parameters)
{
    unsigned int length = 1;
    for (unsigned int i = 0; i < parameters.size(); ++i) {
	if (parameters[i]->length() > 1) {
	    if (length == 1) {
		length = parameters[i]->length();
	    }
	    else if (length != parameters[i]->length()) {
		throw logic_error("Incompatible vector args in VSLogicalNode");
	    }
	}
    }
    return length;
}

static vector<bool> mkIsVector(vector<Node const *> const &parameters)
{
    vector<bool> ans(parameters.size());
    for (unsigned int i = 0; i < parameters.size(); ++i) {
	ans[i] = (parameters[i]->length() > 1);
    }
    return ans;
}

VSLogicalNode::VSLogicalNode(ScalarFunction const *function, 
			     vector<Node const *> const &parameters)
    : LogicalNode(vector<unsigned int>(1,mkLength(parameters)), parameters, 
		  function),
      _func(function), _isvector(mkIsVector(parameters))
{
    if (isObserved()) {
	for (unsigned int ch = 0; ch < _nchain; ++ch) {
	    deterministicSample(ch);
	}
    }
}

void VSLogicalNode::deterministicSample(unsigned int chain)
{
    double *ans = _data + chain * _length;
    vector<double const *> par(_parameters[chain]);
	
    for (unsigned int i = 0; i < _length; ++i) {
	ans[i] = _func->evaluate(par);
	for (unsigned int j = 0; j < par.size(); ++j) {
	    if (_isvector[j])
		++par[j];
	}
    }
}

bool VSLogicalNode::checkParentValues(unsigned int chain) const
{
    vector<double const *> par(_parameters[chain]);

    for (unsigned int i = 0; i < _length; ++i) {
	if (!_func->checkParameterValue(par))
	    return false;
	for (unsigned int j = 0; j < par.size(); ++j) {
	    if (_isvector[j])
		++par[j];
	}
    }
    return true;
}

Node *VSLogicalNode::clone(vector<Node const*> const &parents) const
{
    return new VSLogicalNode(_func, parents);
}


