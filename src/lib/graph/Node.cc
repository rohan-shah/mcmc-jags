#include <config.h>
#include <graph/Node.h>
#include <graph/NodeError.h>
#include <util/nainf.h>
#include <util/dim.h>

#include <stdexcept>
#include <algorithm>

using std::set;
using std::string;
using std::vector;
using std::logic_error;
using std::copy;
using std::find;

class DeterminsticNode;
class StochasticNode;

Node::Node(vector<unsigned int> const &dim, unsigned int nchain)
    : _parents(0), _stoch_children(0), _dtrm_children(0), 
      _dim(getUnique(dim)), _length(product(dim)), _nchain(nchain), _data(0)
      
{
    if (nchain==0)
	throw logic_error("Node must have at least one chain");

    unsigned int N = _length * _nchain;
    _data = new double[N];
    for (unsigned int i = 0; i < N; ++i) {
	_data[i] = JAGS_NA;
    }

    _dtrm_children = new set<DeterministicNode*>;
    _stoch_children = new set<StochasticNode*>;
}

Node::Node(vector<unsigned int> const &dim, 
	   vector<Node const *> const &parents)
    : _parents(parents), _stoch_children(0), _dtrm_children(0), 
      _dim(getUnique(dim)), _length(product(dim)),
      _nchain(countChains(parents)), _data(0)
{
    if (nchain() == 0) {
	throw logic_error("chain number mismatch in Node constructor");
    }

    /*
    for (unsigned int i = 0; i < parents.size(); ++i) {
	if (parents[i] == this) {
	    throw runtime_error("Node cannot be its own parent");
	}
    }
    */
  
    unsigned int N = _length * _nchain;
    _data = new double[N];
    for (unsigned int i = 0; i < N; ++i) {
	_data[i] = JAGS_NA;
    }
  
    _stoch_children = new set<StochasticNode*>;
    _dtrm_children = new set<DeterministicNode*>;
}

Node::~Node()
{
    delete [] _data;
    delete _stoch_children;
    delete _dtrm_children;
}

vector <Node const *> const &Node::parents() const
{
    return _parents;
}

set<StochasticNode*> const *Node::stochasticChildren() 
{
    return _stoch_children;
}

set<DeterministicNode*> const *Node::deterministicChildren() 
{
    return _dtrm_children;
}

static bool isInitialized(Node const *node, unsigned int n)
{
    double const *value = node->value(n);
    for (unsigned int i = 0; i < node->length(); ++i) {
	if (value[i] == JAGS_NA) 
	    return false;
    }
    return true;
}

bool Node::initialize(unsigned int n)
{
    // Test whether node is already initialized and, if so, skip it
    if (isInitialized(this, n))
        return true;

    // Check that parents are initialized
    for (unsigned int i = 0; i < _parents.size(); ++i) {
        if (!isInitialized(_parents[i], n)) {
	    return false; // Uninitialized parent
        }
    }

    deterministicSample(n);

    return true; 
}
    
unsigned int Node::nchain() const
{
  return _nchain;
}

unsigned int countChains(vector<Node const *> const &parameters)
{
  unsigned int nchain = parameters[0]->nchain();

  for (unsigned int i = 1; i < parameters.size(); ++i) {
    if (parameters[i]->nchain() != nchain) {
      nchain = 0;
      break;
    }
  }

  return nchain;
}

void Node::setValue(double const *value, unsigned int length, unsigned int chain)
{
   if (length != _length)
      throw NodeError(this, "Length mismatch in Node::setValue");
   if (chain >= _nchain)
      throw NodeError(this, "Invalid chain in Node::setValue");

   copy(value, value + _length, _data + chain * _length);
}

void Node::swapValue(unsigned int chain1, unsigned int chain2)
{
    double *value1 = _data + chain1 * _length;
    double *value2 = _data + chain2 * _length;
    for (unsigned int i = 0; i < _length; ++i) {
	double v = value1[i];
	value1[i] = value2[i];
	value2[i] = v;
    }
}

/*
bool Node::isDiscreteValued() const
{
    return _isdiscrete;
}

void Node::setDiscreteValued()
{
    _isdiscrete = true;
}
*/

double const *Node::value(unsigned int chain) const
{
    return _data + chain * _length;
}

vector<unsigned int> const &Node::dim() const
{
    return _dim;
}

unsigned int Node::length() const
{
    return _length;
}

void Node::addChild(DeterministicNode *node) const
{
    _dtrm_children->insert(node);
}

void Node::addChild(StochasticNode *node) const
{
    _stoch_children->insert(node);
}

void Node::removeChild(DeterministicNode *node) const
{
    _dtrm_children->erase(node);
}

void Node::removeChild(StochasticNode *node) const
{
    _stoch_children->erase(node);
}
    


