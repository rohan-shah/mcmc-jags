/* Compiler rules for Mixture nodes.

   Node * getMixtureNode(ParseTree const * var, Compiler *compiler);

   This is painfully complicated, but the aim is to be as efficient as
   possible in the allocation of Mixture Nodes.

   Suppose we have an expression X[Y], where X is a vector node of length
   N, and Y is a discrete-valued scalar node. We know that Y can only
   legally take values 1, ..., N. So the simplest approach is to create
   a Mixture Node that takes value X[i] when Y = i, for i = 1, ... N.
   This mixture node has N+1 parents: the index node Y and the N subset
   nodes X[1], ... X[N].

   It may not be efficient to do this in cases where the range of Y is
   restricted.  For example, suppose Y only takes values in the range M1,
   ..., M2, where M1 > 1 and M2 < N.  Then the parents X[1], ... X[M1 -
   1] and X[M2 + 1] ... X[N] are redundant. This may be harmless, but
   there are circumstances under which it will lead to a compiler error.

   There are limited circumstances under which we can calculate the
   possible values of the index node. Currently, this is restricted to
   the case where all the direct Stochastic ancestors of Y are discrete
   and univariate. In this case, the function getMixtureNode1 is used
   to create an efficient Mixture Node. If this fails, we fall back on
   the default getMixtureNode2 function.
*/

#include <compiler/Compiler.h>
#include <compiler/ParseTree.h>
#include <model/SymTab.h>
#include <graph/StochasticNode.h>
#include <model/NodeArray.h>
#include <graph/MixtureNode.h>
#include <graph/GraphMarks.h>
#include <graph/NodeError.h>
#include <sampler/Sampler.h>
#include <util/nainf.h>

#include "MixCompiler.h"

#include <stdexcept>
#include <string>
#include <set>
#include <climits>
#include <algorithm>

using std::vector;
using std::pair;
using std::logic_error;
using std::runtime_error;
using std::string;
using std::set;
using std::map;
using std::copy;
using std::reverse;

//Structure to hold subset indices
struct SSI {
  Node const *node;
  int lower;
  int upper;
  SSI() : node(0), lower(0), upper(0) {};
};


/* 
   Given a vector of subset indices, this function modifies the argument
   "subsets", so that it contains a vector of pairs:
   - Index: Possible value of the variable indices
   - Range: Corresponding range of the array to take as a subset

   All possible values of Index are included in the vector, based on the
   the range of the array we are taking subsets from (default_range).
*/
static void getSubsetRanges(vector<pair<vector<int>, Range> > &subsets,  
			    vector<SSI> const &limits,
			    Range const &default_range)
{
  unsigned int ndim = limits.size();
  //Count number of variable indices (nvi)
  int nvi = 0;
  for (unsigned int j = 0; j < ndim; ++j) {
    if (limits[j].node != 0)
      ++nvi;
  }

  // Create upper and lower bounds
  vector<int> variable_offset(nvi,1), variable_lower(nvi,1), variable_upper(nvi,1);
  vector<int> lower_index(ndim,1), upper_index(ndim,1);
  int k = 0;
  for (unsigned int j = 0; j < ndim; ++j) {
    if (limits[j].node != 0) {
      variable_offset[k] = j;
      variable_lower[k] = default_range.lower()[j];
      variable_upper[k] = default_range.upper()[j];
      ++k;
    }
    else {
      lower_index[j] = limits[j].lower;
      upper_index[j] = limits[j].upper;
    }
  }
  
  for (RangeIterator i(Range(variable_lower, variable_upper)); !i.atEnd(); 
       i.nextLeft()) {
    for (int k = 0; k < nvi; ++k) {
      lower_index[variable_offset[k]] = i[k];
      upper_index[variable_offset[k]] = i[k];
    }
    subsets.push_back(pair<vector<int>, Range>(i, Range(lower_index, upper_index)));
  }
}

//  Returns true if node, or one of its deterministic descendants, is
//  in the set target_nodes
static bool hasDescendant(DeterministicNode *node, 
			  set<Node const*> const &target_nodes,
			  vector<DeterministicNode*> &dnodes,
			  set<DeterministicNode*> &known_dnodes)
{
    if (known_dnodes.count(node))
	return true;

    if (target_nodes.count(node)) {
	known_dnodes.insert(node);
	dnodes.push_back(node);
	return true;
    }

    bool ans = false;
    set<DeterministicNode*> const *dc = node->deterministicChildren();
    for(set<DeterministicNode*>::const_iterator p = dc->begin();
	p != dc->end(); ++p)
    {
	if (hasDescendant(*p, target_nodes, dnodes, known_dnodes)) {
	    ans = true;
	}
    }

    if (ans) {
	known_dnodes.insert(node);
	dnodes.push_back(node);
    }
    return ans;
}

//  Find stochastic parents of given set of nodes within the model,
//  and their intermediate deterministic nodes in forward-sampling order
//  We are looking for discrete, univariate parents.
static bool findStochasticIndices(vector<Node const *> const &tgt_nodes, 
				  Model const &model,
				  vector<StochasticNode *> &stoch_parents,
				  vector<DeterministicNode *> &dtrm_parents)
{
    set<Node const *> tgt_set;
    for (unsigned int i = 0; i < tgt_nodes.size(); ++i) {
	tgt_set.insert(tgt_nodes[i]);
    }
    
    set<DeterministicNode*> known_dnodes;
    vector<StochasticNode*> const &snodes = model.stochasticNodes();
    for (unsigned int i = 0; i < snodes.size(); ++i) {
	/*
	  if (snodes[i]->isObserved())
	  continue;
	*/
	if (tgt_set.count(snodes[i])) {
	    stoch_parents.push_back(snodes[i]);
	}
	else {
	    if (snodes[i]->length() != 1 || !snodes[i]->isDiscreteValued() ||
		!isSupportFixed(snodes[i]))
	    {
		//We are only interested if all the stochastic parents
		//are discrete, univariate, unbounded.
		return false;
	    }

	    bool ans = false;
	    set<DeterministicNode*> const *dc = 
		snodes[i]->deterministicChildren();
	    for(set<DeterministicNode*>::const_iterator p = dc->begin();
		p != dc->end(); ++p)
	    {
		if (hasDescendant(*p, tgt_set, dtrm_parents, known_dnodes)) {
		    ans = true;
		}
	    }
	    if (ans) {
		stoch_parents.push_back(snodes[i]);
		if (stoch_parents.size() > 10) {
		    //This algorithm grinds to a halt with too many
		    //stochastic parents. So bail out after 10
		    return 0;
		}
	    }
	}
    }
    
    /* 
       Deterministic nodes have been pushed back onto the vector 
       dtrm_parents in reverse sampling order.
    */
    reverse(dtrm_parents.begin(), dtrm_parents.end());
    return true;
}

static void cloneNodes(vector<StochasticNode*> const &nodes, 
		       vector<StochasticNode*> &newnodes,
		       map<Node const*, Node const*> &remap)
{
    // Clone nodes without remapping parents
    for (unsigned int i = 0; i < nodes.size(); ++i) {
	StochasticNode *newnode = nodes[i]->clone(nodes[i]->parents());
	newnodes.push_back(newnode);
	remap[nodes[i]] = newnode;
    }
}

static void cloneNodes(vector<DeterministicNode*> const &nodes, 
		       vector<DeterministicNode*> &newnodes,
		       map<Node const*, Node const*> &remap)
{
    // Clone nodes with remapped parents
    for (unsigned int i = 0; i < nodes.size(); ++i) {
	vector<Node const *> args = nodes[i]->parents();
	for (unsigned int j = 0; j < args.size(); ++j) {
	    map<Node const*, Node const*>::const_iterator p =
		remap.find(args[j]);
	    if (p != remap.end()) 
		args[j] = p->second;
	}
	DeterministicNode *newnode = nodes[i]->clone(args);
	newnodes.push_back(newnode);
	remap[nodes[i]] = newnode;
    }
}

static Node* 
getMixtureNode1(NodeArray *array, vector<SSI> const &limits, Compiler *compiler)
// Try to simplify the mixture node by enumerating all possible values
// that the indices can take. This can only be done under certain
// conditions.
{
    unsigned int ndim = limits.size();

    vector<Node const*> indices;
    unsigned int nvi = 0;
    for (unsigned int i = 0; i < ndim; ++i) {
	if (limits[i].node) {
	    indices.push_back(limits[i].node);
	    ++nvi;
	}
    }

    vector<StochasticNode *> sparents;
    vector<DeterministicNode *> dparents;
    if (!findStochasticIndices(indices, compiler->model(), sparents, dparents))
	return 0;

    unsigned int nparents = sparents.size();  
    vector<int> lower(nparents), upper(nparents);
    for (unsigned int i = 0; i < nparents; ++i) {
	StochasticNode const *snode = sparents[i];
	
	// Get lower and upper limits of support
	double l = JAGS_NEGINF, u = JAGS_POSINF;
	snode->support(&l, &u, 1U, 0);
	if (!jags_finite(l) || !jags_finite(u)) {
	    return 0; //Unbounded parent => serious trouble
	}
	if (l < -INT_MAX || u > INT_MAX) {
	    return 0; //Can't cast to int
	}
	lower[i] = static_cast<int>(l);
	upper[i] = static_cast<int>(u);
    }

    map<Node const*, Node const*> remap;
    vector<StochasticNode *> rep_sparents;
    cloneNodes(sparents, rep_sparents, remap);
    
    vector<DeterministicNode const*> dtrm_parents;
    vector<DeterministicNode*> rep_dparents;
    cloneNodes(dparents, rep_dparents, remap);

    vector<Node const*> rep_indices(indices.size());
    for (unsigned int i = 0; i < indices.size(); ++i) {
	rep_indices[i] = remap[indices[i]];
    }

    // Create a set containing all possible values that the stochastic
    // indices can take

    set<vector<int> > index_values;
    vector<int>  this_index(indices.size(),1);
    Range stoch_node_range(lower, upper);

    for (RangeIterator i(stoch_node_range); !i.atEnd(); i.nextLeft()) {

	for (unsigned int j = 0; j < sparents.size(); ++j) {
	    double v = i[j];
	    rep_sparents[j]->setValue(&v, 1, 0);
	}

	for (unsigned int k = 0; k < dparents.size(); ++k) {
	    rep_dparents[k]->deterministicSample(0);
	}
    
	for (unsigned int l = 0; l < indices.size(); ++l) {
	    this_index[l] = static_cast<int>(*rep_indices[l]->value(0));
	}
    
	index_values.insert(this_index);
    }
    
    // We are done with the replicate nodes
    rep_indices.clear();
    while (!rep_dparents.empty()) {
	delete rep_dparents.back();
	rep_dparents.pop_back();
    }
    while (!rep_sparents.empty()) {
	delete rep_sparents.back();
	rep_sparents.pop_back();
    }

    // Now set up the possible subsets defined by the stochastic indices

    vector<int> variable_offset(nvi, 1);
    vector<int> lower_index(ndim, 1), upper_index(ndim, 1);
    int k = 0;
    for (unsigned int j = 0; j < ndim; ++j) {
	if (limits[j].node != 0) {
	    variable_offset[k++] = j;
	}
	else {
	    lower_index[j] = limits[j].lower;
	    upper_index[j] = limits[j].upper;
	}
    }

    vector<pair<vector<int>, Range> > ranges;  
    set<vector<int> >::const_iterator p;
    for (p = index_values.begin(); p != index_values.end(); ++p) {

	vector<int> const &i = *p;
	for (unsigned int k = 0; k < nvi; ++k) {
	    lower_index[variable_offset[k]] = i[k];
	    upper_index[variable_offset[k]] = i[k];
	}
	ranges.push_back(pair<vector<int>, Range>(i, Range(lower_index, upper_index)));
    }

    //Look out for trivial mixture nodes in which all subsets are the same.
    bool trivial = true;
    Node *subset_node0 = array->getSubset(ranges[0].second, compiler->model());

    map<vector<int>, Node const *> subsets;  
    for (unsigned int i = 0; i < ranges.size(); ++i) {
	Node *subset_node = array->getSubset(ranges[i].second, 
					     compiler->model());
	if (!subset_node)
	    return 0;
	subsets[ranges[i].first] = subset_node;
	if (subset_node != subset_node0)
	    trivial = false;
    }

    if (trivial) {
	// Nothing to do here! All subset nodes are the same, so we
	// just return the subset node.
	return subset_node0;
    }

    return compiler->mixtureFactory1().getMixtureNode(indices, subsets, 
						      compiler->model());
}

static Node * 
getMixtureNode2(NodeArray *array, vector<SSI> const &limits, Compiler *compiler)
{
    vector<pair<vector<int>, Range> > ranges;  
    getSubsetRanges(ranges, limits, array->range());

    map<vector<int>, Node const *> subsets;
    for (unsigned int i = 0; i < ranges.size(); ++i) {
	Node *subset_node =   array->getSubset(ranges[i].second, 
					       compiler->model());
	if (!subset_node)
	    return 0;
	subsets[ranges[i].first] = subset_node;
    }

    vector<Node const *> indices;
    for (unsigned int i = 0; i < limits.size(); ++i) {
	if (limits[i].node) {
	    indices.push_back(limits[i].node);
	}
    }

    return compiler->mixtureFactory2().getMixtureNode(indices, subsets, 
						      compiler->model());
}

Node * getMixtureNode(ParseTree const * var, Compiler *compiler)
{
    if (var->treeClass() != P_VAR) {
	throw logic_error("Expecting variable expression");
    }
  
    NodeArray *array = compiler->model().symtab().getVariable(var->name());
    if (array == 0) {
	throw runtime_error(string("Unknown parameter: ") + var->name());
    }

    vector<ParseTree*> const &range_list = var->parameters();
    vector<SSI> limits;
    unsigned int ndim = array->range().ndim(false);
    if (range_list.size() != ndim) {
	throw runtime_error("Dimension mismatch taking variable subset of " + 
			    var->name());
    }

    unsigned int nvi = 0; //Count number of variable indices
    for (unsigned int i = 0; i < ndim; ++i) {
	ParseTree const *range_element = range_list[i];
	if (range_element->treeClass() != P_RANGE) {
	    throw runtime_error("Malformed range expression");
	}
	SSI ssi;
	ssi.node = 0;
	ParseTree const *p0, *p1;
	switch(range_element->parameters().size()) {
	case 0:
	    // Index is empty, implying the whole range 
	    ssi.lower = array->range().lower()[i];
	    ssi.upper = array->range().upper()[i];
	    break;
	case 1:
	    // Single index: upper = lower
	    p0 = range_element->parameters()[0];
	    if(compiler->indexExpression(p0, ssi.lower)) {
		ssi.upper = ssi.lower;
	    }
	    else {
		ssi.node = compiler->getParameter(p0);
		if (ssi.node == 0)
		    return 0;
		else
		    ++nvi;
	    }
	    break;
	case 2:
	    // Upper and lower indices
	    p0 = range_element->parameters()[0];
	    p1 = range_element->parameters()[1];
	    if(compiler->indexExpression(p0, ssi.lower)) {
		if (!compiler->indexExpression(p1, ssi.upper)) {
		    return 0;
		}
	    }
	    else {
		ssi.node = compiler->getParameter(p0);
		if (compiler->getParameter(p1) != ssi.node)
		    return 0;
		else
		    ++nvi;
	    }
	    break;
	default:
	    throw logic_error("Invalid range expression");
	}
	//Check validity of subset index
	if (ssi.node) {
	    if (!ssi.node->isDiscreteValued()) {
		throw NodeError(ssi.node, "Continuous node used as index");
	    }
	    if (ssi.node->length() != 1) {
		throw NodeError(ssi.node, "Vector node used as index");
	    }
	}
	else {
	    if (ssi.lower < array->range().lower()[i] ||
		ssi.upper > array->range().upper()[i] ||
		ssi.upper < ssi.lower)
	    {
		throw runtime_error("Requested invalid variable subset of " + 
				    var->name());
	    }
    }
	limits.push_back(ssi);
}
  
    //Check number of variable indices (nvi)
    if (nvi == 0) {
	throw logic_error("Trivial mixture node");
    }

Node *mnode = getMixtureNode1(array, limits, compiler);
if (mnode)
    return mnode;
else
    return getMixtureNode2(array, limits, compiler);
}
