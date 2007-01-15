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

static vector<Node const *> 
mkParents(vector<Node const *> const &index,
	  vector<pair<vector<int>, Node const *> > const &param)
{
  vector<Node const *> parents;
  parents.reserve(index.size() + param.size());
  for (unsigned int i = 0; i < index.size(); ++i) {
    parents.push_back(index[i]);
  }
  for (unsigned int i = 0; i < param.size(); ++i) {
    parents.push_back(param[i].second);
  }
  return parents;
}

//debuggin
#include <iostream>

MixtureNode::MixtureNode(vector<Node const *> const &index,
			 vector<pair<vector<int>, Node const *> > const &parameters)
  : DeterministicNode(parameters[0].second->dim(),
		      mkParents(index, parameters)),
		      _Nindex(index.size())
{
  for (vector<Node const *>::const_iterator i = index.begin(); 
       i != index.end(); ++i)
    {
      Node const *node = *i;
      if (node->length() != 1 || !node->isDiscreteValued()) {
         throw NodeError(node, "Invalid index parameter for mixture node");
      }
    }

  unsigned int ndim = parameters.size();
  for (unsigned int i = 0; i < ndim; ++i) {
    Node const *node = parameters[i].second;
    if (!node) {
      throw invalid_argument("Null parameter in MixtureNode");
    }
    if (parameters[i].first.size() != _Nindex) {
      std::cout << "Expected " << parameters[i].first.size() << " got " <<
                  _Nindex << "\n";
      throw invalid_argument("Invalid index in MixtureNode");
    }
    _map.insert(parameters[i]);
  }
  
  bool isdiscrete = true;
  vector<unsigned int> const &default_dim = dim();
  for (unsigned int i = 0; i < ndim; ++i) {
    Node const *node = parameters[i].second;
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
    map<vector<int>, Node const*>::iterator p = _map.find(i);
    if (p != _map.end()) {
	setValue(p->second->value(chain), p->second->length(), chain);
    }
    else {
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
        throw NodeError(this, "Invalid index");
        */
	throw logic_error("Invalid index in MixtureNode");
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
    vector<int> i(_Nindex);
    vector <Node const*> const &par = parents();
    for (unsigned int j = 0; j < _Nindex; ++j) {
	i[j] = static_cast<int>(*par[j]->value(chain));
    }
    return _map.find(i) != _map.end();
}
