#include <config.h>
#include <graph/Node.h>
#include <graph/NodeError.h>
#include <graph/NodeNameTab.h>
#include <sarray/nainf.h>
#include <sarray/util.h>

#include <stdexcept>
#include <algorithm>

using std::set;
using std::string;
using std::vector;
using std::logic_error;
using std::copy;
using std::find;

Node::Node(vector<unsigned int> const &dim, unsigned int nchain)
    : _parents(0), _children(0), _ref(0), _isobserved(false),
      _isdiscrete(false), _dim(dim), _length(product(dim)), _nchain(nchain),
      _data(0)
      
{
    if (nchain==0)
	throw logic_error("Node must have at least one chain");

    unsigned int N = _length * _nchain;
    _data = new double[N];
    for (unsigned int i = 0; i < N; ++i) {
	_data[i] = JAGS_NA;
    }

    _children = new set<Node*>;
}

Node::Node(vector<unsigned int> const &dim, 
	   vector<Node const *> const &parents)
  : _parents(parents), _children(0), _ref(0),  _isobserved(false), 
    _isdiscrete(false), _dim(dim), _length(product(dim)),
     _nchain(countChains(parents)), _data(0)
{
  if (nchain() == 0) {
    throw logic_error("chain number mismatch in Node constructor");
  }

  for (unsigned int i = 0; i < parents.size(); ++i) {
    if (parents[i] == this) {
      throw NodeError(this, "Node cannot be its own parent");
    }
  }
  
  for (unsigned int i = 0; i < parents.size(); ++i) {
    parents[i]->_children->insert(this);
  }

  unsigned int N = _length * _nchain;
  _data = new double[N];
  for (unsigned int i = 0; i < N; ++i) {
      _data[i] = JAGS_NA;
  }
  
  _children = new set<Node*>;
}

Node::~Node()
{
    delete [] _data;

    for (unsigned int i = 0; i < _parents.size(); ++i) {
	_parents[i]->_children->erase(this);
    }
    for (set<Node*>::iterator p = _children->begin(); p != _children->end(); 
	 p++)
	{
	    vector<Node const *> &P = (*p)->_parents;
	    vector<Node const *>::iterator cp;
	    while ( (cp = find(P.begin(), P.end(), this)) != P.end()) {
		P.erase(cp);
	}
    }

    delete _children;
}

void Node::ref()
{
  _ref++;
}

void Node::unref()
{
  _ref--;
  if (_ref == 0) {
    delete this;
  }
}

unsigned int Node::refCount() const
{
  return _ref;
}

vector <Node const *> const &Node::parents() const
{
  return _parents;
}

set<Node*> const *Node::children() 
{
  return _children;
}

static bool isInitialized(Node const *node)
{
   for (unsigned int n = 0; n < node->nchain(); ++n) {
       double const *value = node->value(n);
       for (unsigned int i = 0; i < node->length(); ++i) {
           if (value[i] == JAGS_NA) 
               return false;
       }
   }
   return true;
}

bool Node::initialize()
{
    // Test whether node is already initialized and, if so, skip it
    if (isInitialized(this))
        return true;

    // Check that parents are initialized
    for (unsigned int i = 0; i < _parents.size(); ++i) {
        if (!isInitialized(_parents[i])) {
	    return false; // Uninitialized parent
        }
    }
    for (unsigned int n = 0; n < _nchain; ++n) {
        deterministicSample(n);
    }

    if (isVariable())
        return true; // Nothing more to do for variable nodes

    //Check if non-variable node is a function
    //of observed parents. In this case, it is observed also 
    for (unsigned int i = 0; i < _parents.size(); ++i) {
	if (!_parents[i]->isObserved()) {
            return true; //Not observed
	}
    }
    _isobserved = true;
    return true;
}
    
string Node::name(NodeNameTab const &name_table) const
{
  return name_table.getName(this);
}

void Node::setObserved(double const *value, unsigned int length)
{
    if (length != _length) {
	throw logic_error("Length mismatch in Node::setObserved");
    }

    for (unsigned int n = 0; n < _nchain; ++n) {
        for (unsigned int i = 0; i < length; ++i) {
	    _data[n * length + i] = value[i];
        }
    }
    _isobserved = true;
}

bool Node::isObserved() const
{
    return _isobserved;
}

unsigned int Node::nchain() const
{
  return _nchain;
}

unsigned int countChains(std::vector<Node const *> const &parameters)
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
/*
   double *v = _data + length * chain;
   for (unsigned int i = 0; i < length; ++i) {
      v[i] = value[i];
   }
*/
}

bool Node::isDiscreteValued() const
{
    return _isdiscrete;
}

void Node::setDiscreteValued()
{
    _isdiscrete = true;
}

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
