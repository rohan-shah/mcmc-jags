#include <config.h>
#include <graph/AggNode.h>

#include <vector>
#include <stdexcept>

using std::vector;
using std::set;

AggNode::AggNode(vector<unsigned int> const &dim, 
		 vector<Node const *> const &nodes,
                 vector<unsigned int> const &offsets)
    : DeterministicNode(dim, nodes), _offsets(offsets)
{
  /* Check argument lengths */
  if (_length != nodes.size() || _length != offsets.size()) {
    throw std::length_error ("Length mismatch in Aggregate Node constructor");
  }

  /* Check that offsets are valid */
  for (unsigned int i = 0; i < _length; i++) {
    if (offsets[i] >= nodes[i]->length())
      throw std::out_of_range("Invalid offset in Aggregate Node constructor");
  }
  
  /* See if node is discrete-valued */
  bool isdiscrete = true;
  for (unsigned int i = 0; i < _length; i++) {
    if (!nodes[i]->isDiscreteValued()) {
      isdiscrete = false;
      break;
    }
  }
  if (isdiscrete) {
    setDiscreteValued();
  }
}

AggNode::~AggNode()
{
}

void AggNode::deterministicSample(unsigned int chain)
{
  double *value = _data + _length * chain;
  vector<Node const*> const &par = parents();
  for (unsigned int i = 0; i < _length; ++i) {
    value[i] = par[i]->value(chain)[_offsets[i]];
  }
}

AggNode const *asAggregate(Node *node)
{
  return dynamic_cast<AggNode const*>(node);
}

bool AggNode::isLinear(set<Node const*> const &parameters, bool fixed) const
{
   return true;
}

bool AggNode::isScale(set<Node const *> const &parameters, bool fixed) const
{
    return (parents().size() == 1);
}
