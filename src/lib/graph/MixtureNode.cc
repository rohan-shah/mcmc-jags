#include <config.h>
#include <graph/MixtureNode.h>

#include <utility>
#include <vector>
#include <stdexcept>

#include <graph/NodeError.h>

using std::pair;
using std::vector;
using std::map;
using std::invalid_argument;
using std::logic_error;
using std::set;

static vector<unsigned int> const &
mkDim(map<vector<int>, Node const *> const &param)
{
    map<vector<int>, Node const *>::const_iterator p = param.begin();
    vector<unsigned int> const &dim = p->second->dim();
    for (++p ; p != param.end(); ++p) {
	if (p->second->dim() != dim) {
	    throw logic_error("Dimension mismatch in MixtureNode parents");
	}
    }
    return dim;
}

static vector<Node const *> 
mkParents(vector<Node const *> const &index,
	  map<vector<int>, Node const *> const &param)
{
    vector<Node const *> parents;
    parents.reserve(index.size() + param.size());
    for (unsigned int i = 0; i < index.size(); ++i) {
	parents.push_back(index[i]);
    }
    for (map<vector<int>, Node const *>::const_iterator p = param.begin();
	 p != param.end(); ++p) 
    {
	parents.push_back(p->second);
    }
    return parents;
}

MixtureNode::MixtureNode (vector<Node const *> const &index,
			  map<vector<int>, Node const *> const &parameters)
    : DeterministicNode(mkDim(parameters), mkParents(index, parameters)),
      _map(parameters), _Nindex(index.size())
{
    for (vector<Node const *>::const_iterator i = index.begin(); 
	 i != index.end(); ++i)
    {
	Node const *node = *i;
	if (node->length() != 1 || !node->isDiscreteValued()) {
	    throw NodeError(node, "Invalid index parameter for mixture node");
	}
    }

    bool isdiscrete = true;
    vector<unsigned int> const &default_dim = parameters.begin()->second->dim();
    
    for (map<vector<int>, Node const *>::const_iterator p = parameters.begin();
	 p != parameters.end(); ++p)
    {
	if (p->first.size() != _Nindex) {
	    throw invalid_argument("Invalid index in MixtureNode");
	}
	Node const *node = p->second;
	if (node->dim() != default_dim) {
	    throw invalid_argument("Range mismatch for MixtureNode parameters");
	}
	if (!node->isDiscreteValued()) {
	    isdiscrete = false;
	}
    }

    if (isdiscrete) {
	setDiscreteValued();
    }
}

MixtureNode::~MixtureNode()
{
}

/* Do not delete commented sections: they are useful for debugging
#include <iostream>
#include <sarray/Range.h>
#include <sarray/nainf.h>
#include <graph/NodeError.h>
*/
void MixtureNode::deterministicSample(unsigned int chain)
{
    vector<int> i(_Nindex);
    vector <Node const*> const &par = parents();
    for (unsigned int j = 0; j < _Nindex; ++j) {
	i[j] = static_cast<int>(*par[j]->value(chain));
    }
    map<vector<int>, Node const *>::const_iterator p = _map.find(i);
    if (p == _map.end()) {
	/*
	std::cout << "Got " << print(Range(i)) << "\nOriginally\n";
	for (unsigned int j = 0; j < _Nindex; ++j) {
	    std::cout << par[j]->value(chain)[0] << "\n";
	    if (par[j]->value(chain)[0] == JAGS_NA)
		std::cout << "(which is  missing)\n";
	}
	std::cout << "Expected one of \n";
	for (p = _map.begin(); p != _map.end(); ++p) {
	    std::cout << print(Range(p->first)) << "\n";
	}
	*/
	throw NodeError(this, "Invalid index in MixtureNode");
    }
    else {
	setValue(p->second->value(chain), length(), chain);	
    }
    }

unsigned int MixtureNode::index_size() const
{
  return _Nindex;
}

MixtureNode const *asMixture(Node const *node)
{
  return dynamic_cast<MixtureNode const*>(node);
}

bool isMixture(Node const *node)
{
  return dynamic_cast<MixtureNode const*>(node);
}

bool 
MixtureNode::isLinear(set<Node const*> const &parameters, bool fixed) const
{
  if (fixed)
      return false;

  //Check that none of the indices are in the parameter set
  vector<Node const*> const &par = parents();
  for (unsigned int i = 0; i < _Nindex; ++i) {
    if (parameters.count(par[i]))
      return false;
  }
  return true;
}

bool MixtureNode::isScale(set<Node const*> const &parameters, bool fixed) const
{
    return isLinear(parameters, fixed);
}

bool MixtureNode::checkParentValues(unsigned int chain) const
{
    /* 
       We ignore the index nodes. There there are no restrictions on
       the values of the other parents, and the function trivially
       returns true.

       In principle, we could check whether the index nodes form a
       valid index. However, this calculation has to be done anyway as
       part of the calculation for deterministicSample, there is not
       much point repeating it here.

       This may be considered cheating, but it avoids doing the same
       calculation twice.
    */
       
    return true;
}
