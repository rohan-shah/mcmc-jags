
#include <config.h>
#include <graph/AggNode.h>
#include <graph/GraphMarks.h>
#include <graph/Graph.h>

#include <vector>
#include <stdexcept>

using std::vector;
using std::set;
using std::logic_error;
using std::string;


AggNode::AggNode(vector<unsigned int> const &dim, 
		 vector<Node const *> const &parents,
                 vector<unsigned int> const &offsets)
    : DeterministicNode(dim, parents), _offsets(offsets),
      _parent_values(_length * _nchain)
{
  /* Check argument lengths */
  if (_length != parents.size() || _length != offsets.size()) {
    throw std::length_error ("Length mismatch in Aggregate Node constructor");
  }

  /* Check that offsets are valid */
  for (unsigned int i = 0; i < _length; i++) {
    if (offsets[i] >= parents[i]->length())
      throw std::out_of_range("Invalid offset in Aggregate Node constructor");
  }
  
  /* Setup parent values */
  for (unsigned int ch = 0; ch < _nchain; ++ch) {
      for (unsigned int i = 0; i < _length; ++i) {
	  _parent_values[i + ch * _length] = parents[i]->value(ch) + offsets[i];
      }
  }

  /* Initialize if fully observed */
  if (isObserved()) {
      for (unsigned int ch = 0; ch < _nchain; ++ch) {
	  deterministicSample(ch);
      }
  }

}

AggNode::~AggNode()
{
}

void AggNode::deterministicSample(unsigned int chain)
{
    unsigned int N = _length * chain;
    for (unsigned int i = 0; i < _length; ++i) {
	_data[i + N] = *_parent_values[i + N];
    }
}

AggNode const *asAggregate(Node *node)
{
  return dynamic_cast<AggNode const*>(node);
}

/*
bool AggNode::isLinear(GraphMarks const &linear_marks, bool fixed) const
{
    vector<Node const *> const &par = parents();
    for (unsigned int i = 0; i < par.size(); ++i) {
	if (linear_marks.graph().contains(par[i])) {
	    switch(linear_marks.mark(par[i])) {
	    case MARK_NULL: case MARK_TRUE:
		break;
	    case MARK_FALSE:
		return false;
		break;
	    default:
		throw logic_error("Invalid graphmarks in AggNode::isLinear");
	    }
	}
    }
    return true;
}
*/

bool AggNode::isClosed(set<Node const *> const &ancestors, 
		       ClosedFuncClass fc, bool fixed) const
{
    switch(fc) {
    case DNODE_SCALE:
	//All parents must be scale transformations
	for (unsigned int i = 0; i < parents().size(); ++i) {
	    if (ancestors.count(parents()[i]) == 0) {
		return false;
	    }
	}
	break;
    case DNODE_SCALE_MIX:
	//The aggnode must be a subset so only one distinct parent
	if (ancestors.count(parents()[0]) == 0)
	    return false;
	for (unsigned int i = 1; i < parents().size(); ++i) {
	    if (parents()[i] != parents()[0])
		return false;
	}
	break;
    case DNODE_LINEAR:
	break;
    case DNODE_POWER:
	return false;
	break;
    }
    return true;
}

bool AggNode::checkParentValues(unsigned int) const
{
   return true;
}

string AggNode::deparse(vector<string> const &parents) const
{
    return string("aggregate(") + parents.front() + "..." + 
	parents.back() + ")";
}

Node *AggNode::clone(vector<Node const *> const &parents) const
{
    return new AggNode(this->dim(), parents, _offsets);
}

bool AggNode::isDiscreteValued() const
{
    for (unsigned int i = 0; i < parents().size(); ++i) {
        if (!parents()[i]->isDiscreteValued()) return false;
    }
    return true;
}
