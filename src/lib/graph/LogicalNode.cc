#include <config.h>
#include <graph/LogicalNode.h>
#include <function/Function.h>
#include <graph/NodeError.h>
#include <graph/NodeNameTab.h>
#include <graph/Graph.h>

#include <stdexcept>
#include <vector>
#include <string>
#include <math.h>

using std::vector;
using std::string;
using std::set;
using std::logic_error;
using std::runtime_error;

static vector<unsigned int> mkDim(Function const *func, 
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
  if (!func->checkParameterLength(parameter_dims.size())) {
    //FIXME: logic_error or runtime_error?
    throw runtime_error(string("Incorrect number of parameters for function ")
		      + func->name());
  }
  if (!func->checkParameterDim(parameter_dims)) {
    throw runtime_error(string("Non-conforming parameters for function ")
		      + func->name());
  }
  return func->dim(parameter_dims);
}


LogicalNode::LogicalNode(Function const *function, 
			 vector<Node const *> const &parameters)
  : DeterministicNode(mkDim(function,parameters), parameters),
    _func(function),
    _parameters(nchain())
{
  
    _dims.reserve(parameters.size());
    _lengths.reserve(parameters.size());
    for (unsigned int j = 0; j < parameters.size(); ++j) {
	_dims.push_back(parameters[j]->dim());
	_lengths.push_back(parameters[j]->length());
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

LogicalNode::~LogicalNode()
{
}

string LogicalNode::name(NodeNameTab const &name_table) const
{
  string name = name_table.getName(this);
  if (!name.empty())
    return name;

  vector<Node const *> const &par = parents();

  vector<string> parnames(par.size());
  for (unsigned int i = 0; i < par.size(); ++i) {
      parnames[i] = par[i]->name(name_table);
  }
  
  name.append("(");
  name.append(_func->deparse(parnames));
  name.append(")");
	      
  return name;
}

void LogicalNode::deterministicSample(unsigned int chain)
{
  if (!_func->checkParameterValue(_parameters[chain], _lengths, _dims)) {
    throw NodeError(this, "Invalid parameter values for LogicalNode");
  }
  _func->evaluate(_data + chain * _length, _parameters[chain], _lengths, _dims);
}

LogicalNode const *asLogical(Node const *node)
{
  return dynamic_cast<LogicalNode const*>(node);
}

bool isLogical(Node const *node)
{
  return dynamic_cast<LogicalNode const*>(node);
}

bool LogicalNode::isLinear(set<Node const *> const &parameters, 
			   Graph const &graph, bool fixed) const
{
    vector<bool> mask(parents().size());
    for (unsigned int i = 0; i < parents().size(); ++i) {
	Node const *p = parents()[i];
        if (graph.contains(p)) {
            if (parameters.count(p) == 0) {
                //Parent is a non-linear function. No way to recover.
                return false;
            }
            else {
                mask[i] = true;
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

bool LogicalNode::isScale(set<Node const *> const &parameters, 
			  Graph const &graph, bool fixed) const
{
    unsigned int nparam = 0;
    unsigned int index = 0;
    for (unsigned int i = 0; i < parents().size(); ++i) {
	Node const *p = parents()[i];
	if (graph.contains(p)) {
	    if (parameters.count(p)) {
		nparam++;
		index = i;
	    }
	    else {
		return false; //Parent is non-linear function
	    }
	}
    }
    if (nparam == 0)
	return true;
    else if (nparam > 1)
	return false;
    
    vector<bool> fixed_mask;
    if (fixed) {
	for (unsigned int i = 0; i < parents().size(); ++i) {
	    fixed_mask.push_back(parents()[i]->isObserved());
	}
    }

    return _func->isScale(index, fixed_mask);
}

bool LogicalNode::checkParentValues(unsigned int chain) const
{
    return _func->checkParameterValue(_parameters[chain], _lengths, _dims);
}
