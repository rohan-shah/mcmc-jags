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

static vector<Node const*> &marked_nodes()
{
    // Vector of nodes marked for deletion

    static vector<Node const*> _marked_nodes;
    return _marked_nodes;
}

/*
void Node::checkForDeletion() const
{
    
    //Check to see if node can be delete (reference count of zero and
    //no children). If so, add it to the list of nodes marked for deletion.
    
    if (_ref == 0 && _children()->empty()) {
	marked_nodes().push_back(node);
    }
}
*/

void Node::sweep() {

    //Memory management for nodes

    while (!marked_nodes().empty()) {

	//When nodes are deleted, they may push other nodes onto the 
	//marked list. So we copy it to a new vector.

	vector<Node const*> delete_list = marked_nodes();
	marked_nodes().clear();
	
	for (unsigned int i = 0; i < delete_list.size(); ++i) {
	    delete delete_list[i];
	}
    }
}

Node::Node(vector<unsigned int> const &dim, unsigned int nchain)
    : _parents(0), _children(0), _ref(0), _isobserved(false),
      _isdiscrete(false), _dim(getUnique(dim)), _length(product(dim)), 
      _nchain(nchain),
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
      _isdiscrete(false), _dim(getUnique(dim)), _length(product(dim)),
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
    delete _children;

    for (unsigned int i = 0; i < _parents.size(); ++i) {
	_parents[i]->_children->erase(this);

	if (_parents[i]->_ref == 0 && _parents[i]->_children->empty()) {
	    //marked_nodes().push_back(parents[i]);
	}
    }
}

void Node::ref()
{
    _ref++;
}

void Node::unref()
{
    _ref--;
    if (_ref == 0 && _children->empty()) {
	marked_nodes().push_back(this);
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

static bool isInitialized(Node const *node, unsigned int n)
{
    double const *value = node->value(n);
    for (unsigned int i = 0; i < node->length(); ++i) {
	if (value[i] == JAGS_NA) 
	    return false;
    }
    return true;
}

void Node::initializeData()
{
    if (this->isRandomVariable() || this->isObserved())
	return;
    
    //Test whether all parents are observed
    for (unsigned int i = 0; i < _parents.size(); ++i) {
	if (!_parents[i]->isObserved()) {
            return; //Not observed
	}
    }

    for (unsigned int n = 0; n < _nchain; ++n) {
        deterministicSample(n);
    }
    _isobserved = true;
}
    
bool Node::initialize(RNG *rng, unsigned int n)
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

    bool isfixed = true; //Is it a fixed effect?
    for (unsigned int i = 0; i < _parents.size(); ++i) {
	if (!_parents[i]->isObserved()) {
            isfixed = false; //Not observed
	    break;
	}
    }

    if (isfixed) {
	deterministicSample(n);
    }
    else {
	randomSample(rng, n);
    }
    return true; 
}
    

/*
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
*/

void Node::setObserved(vector<double> const &value)
{
    if (value.size() != _length) {
	throw logic_error("Length mismatch in Node::setObserved");
    }

    for (unsigned int n = 0; n < _nchain; ++n) {
        for (unsigned int i = 0; i < _length; ++i) {
	    _data[n * _length + i] = value[i];
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
