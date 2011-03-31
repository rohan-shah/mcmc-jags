#include <config.h>
#include <graph/MixtureNode.h>
#include <graph/GraphMarks.h>
#include <graph/Graph.h>

#include <utility>
#include <vector>
#include <stdexcept>

#include <graph/NodeError.h>

using std::vector;
using std::map;
using std::invalid_argument;
using std::logic_error;
using std::set;
using std::string;
using std::pair;

static map<MixMap, int> &mixMapMap()
{
    /* 
       Repository of MixMaps that are shared between MixtureNode
       objects.  We use reference counting to keep track of whether a
       MixMap is in use.
    */
    static map<MixMap, int> _mixmapmap;
    return _mixmapmap;
}

static MixMap const &insertMixMap(MixMap const &m)
{
    /*
      Inserts a MixMap into the shared repository if it is unique, or
      increments its reference count if it is already contained in the
      repository.
      
      The return value is a reference to a copy of the mixmap in the
      repository, which will persist as long as we need it.

      N.B. This must be called only by the MixtureNode constructor!
    */

    map<MixMap, int> &mmap = mixMapMap();
    map<MixMap, int>::iterator p = mmap.find(m);
    if (p == mmap.end()) {
	mmap.insert(pair<MixMap,int>(m, 1));
	p = mmap.find(m);
    }
    else {
	p->second++;
    }
    return p->first;
}

static void removeMixMap(MixMap const &m)
{
    /*
      Decrements the reference count of a MixMap in the shared
      repository.  When the reference count reaches zero, the MixMap
      is removed.

      N.B. This must be called only by the MixtureNode destructor!
    */
    
    map<MixMap, int> &mmap = mixMapMap();
    map<MixMap, int>::iterator p = mmap.find(m);
    if (p == mmap.end()) {
	throw logic_error("Failed to find MixMap in MixtureNode");
    }
    else {
	p->second--;
    }

    if (p->second == 0) {
	mmap.erase(p);
    }
}

/*
  Calculates the dimensions of a mixture node given its possible parent
  values. If the parents have inconsistent dimensions, then a logic
  error is thrown.
*/
static vector<unsigned int> const &
mkDim(map<vector<int>, Node const *> const &mixmap)
{
    map<vector<int>, Node const *>::const_iterator p = mixmap.begin();
    vector<unsigned int> const &dim = p->second->dim();
    for (++p ; p != mixmap.end(); ++p) {
	if (p->second->dim() != dim) {
	    throw logic_error("Dimension mismatch in MixtureNode parents");
	}
    }
    return dim;
}

/* 
   Creates a vector of parent nodes from the arguments passed to the
   constructor. 
   
   The index nodes come first, in the order supplied, then the parents
   supplied in the mixmap parameter, in the order determined by the
   corresponding indices.
*/
static vector<Node const *> 
mkParents(vector<Node const *> const &index,
	  map<vector<int>, Node const *> const &mixmap)
{
    vector<Node const *> parents;
    parents.reserve(index.size() + mixmap.size());
    for (unsigned int i = 0; i < index.size(); ++i) {
	parents.push_back(index[i]);
    }
    for (map<vector<int>, Node const *>::const_iterator p = mixmap.begin();
	 p != mixmap.end(); ++p) 
    {
	parents.push_back(p->second);
    }
    return parents;
}

MixtureNode::MixtureNode (vector<Node const *> const &index,
			  map<vector<int>, Node const *> const &mixmap)
    : DeterministicNode(mkDim(mixmap), mkParents(index, mixmap)),
      _map(insertMixMap(mixmap)), _Nindex(index.size()), _discrete(true)
{
    // Check validity of index argument

    if (index.empty())
	throw invalid_argument("NULL index in MixtureNode constructor");

    for (vector<Node const *>::const_iterator i = index.begin(); 
	 i != index.end(); ++i)
    {
	Node const *node = *i;
	if (node->length() != 1 || !node->isDiscreteValued() ||
	    node->isObserved()) 
	{
	    throw invalid_argument("Invalid index in MixtureNode constructor");
	}
    }

    // Check validity of mixmap argument

    if (mixmap.size() < 2)
	throw invalid_argument("Trivial mixmap in MixtureNode constructor");

    for (map<vector<int>, Node const *>::const_iterator p = mixmap.begin();
	 p != mixmap.end(); ++p)
    {
	if (p->first.size() != _Nindex) {
	    throw invalid_argument("Invalid index in MixtureNode");
	}
	//Check discreteness of outcome
	if (!p->second->isDiscreteValued()) {
	  _discrete = false;
	}
    }


}


MixtureNode::~MixtureNode()
{
    removeMixMap(_map);
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

string MixtureNode::deparse(vector<string> const &parents) const
{
    string name = "mixture(index=[";

    vector<int> i(_Nindex);
    for (unsigned int j = 0; j < _Nindex; ++j) {
	if (j > 0) {
	    name.append(",");
	}
	name.append(parents[j]);
    }
    name.append("], parents= ");

    /* We can't list all possible parents in a name, since there is
       no limit on the number. So we take the first and last */
    name.append(parents[_Nindex]); //first parent
    if (parents.size() > _Nindex + 2) {
        name.append("...");
    }
    else {
        name.append(",");
    }
    name.append(parents.back()); //last parent
    name.append(")");
	      
    return name;
}

MixtureNode const *asMixture(Node const *node)
{
  return dynamic_cast<MixtureNode const*>(node);
}

bool isMixture(Node const *node)
{
  return dynamic_cast<MixtureNode const*>(node);
}


bool MixtureNode::isClosed(set<Node const *> const &ancestors, 
			   ClosedFuncClass fc, bool fixed) const
{
    if (fixed)
	return false;

    //Check that none of the indices are in the ancestor set
    vector<Node const*> const &par = parents();
    for (unsigned int i = 0; i < _Nindex; ++i) {
	if (ancestors.count(par[i])) {
	    return false;
	}
    }

    switch(fc) {
    case DNODE_LINEAR: case DNODE_SCALE_MIX: case DNODE_POWER:
	break;
    case DNODE_SCALE:
	//Only a scale function if all possible parents are scale functions
	for (unsigned int i = _Nindex; i < par.size(); ++i) {
	    if (ancestors.count(par[i])==0)
		return false;
	}
	break;
    }
    
    return true;
}

bool MixtureNode::checkParentValues(unsigned int chain) const
{
    return true;
}

DeterministicNode *MixtureNode::clone(vector<Node const *> const &parents) const
{
    vector<Node const *> index(_Nindex);
    vector<Node const *>::const_iterator p = parents.begin();
    for (unsigned int i = 0; i < _Nindex; ++i) {
	index[i] = *p;
	++p;
    }
    
    map<vector<int>, Node const *> mixmap;
    map<vector<int>, Node const *>::const_iterator q = _map.begin();
    while (p != parents.end() && q != _map.end()) {
	mixmap[q->first] = *p;
	++q;
	++p;
    }
    
    return new MixtureNode(index, mixmap);
}

bool MixtureNode::isDiscreteValued() const
{
    return _discrete;
}
