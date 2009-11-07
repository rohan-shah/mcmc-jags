#include <config.h>
#include <graph/LinkNode.h>
#include <function/InverseLinkFunc.h>
#include <util/dim.h>

#include <stdexcept>
#include <vector>
#include <string>
#include <math.h>

class GraphMarks;

using std::vector;
using std::string;
using std::set;
using std::logic_error;
using std::runtime_error;

LinkNode::LinkNode(InverseLinkFunc const *function,  
                   vector<Node const *> const &parameters)
    : DeterministicNode(vector<unsigned int>(1,1), parameters),
      _func(function),
      _parameters(parameters[0]->value(0))
{
    if (!parameters.size() == 1) {
        throw runtime_error("Invalid number of parameters in LinkNode");
    }
    if (!isScalar(parameters[0]->dim())) {
	throw runtime_error("Invalid parameter dims in LinkNode");
    }
}

string LinkNode::deparse(vector<string> const &parents) const
{
    string name = "(";
    name.append(_func->deparse(parents));
    name.append(")");
	      
    return name;
}

void LinkNode::deterministicSample(unsigned int chain)
{
    _data[chain] = _func->inverseLink(_parameters[chain]);
}

bool LinkNode::isClosed(std::set<Node const *> const &ancestors, 
			ClosedFuncClass fc, bool fixed) const
{
    return false;
}

bool LinkNode::checkParentValues(unsigned int chain) const
{
    return true;
}

InverseLinkFunc const * LinkNode::link() const
{
    return _func;
}
   
Node *LinkNode::clone(vector<Node const *> const &parents) const
{
    return new LinkNode(_func, parents);
}

bool LinkNode::isDiscreteValued() const
{
    return false;
}

double LinkNode::eta(unsigned int chain) const
{
    return _parameters[chain];
}

double LinkNode::grad(unsigned int chain) const
{
    return _func->grad(_parameters[chain]);
}
