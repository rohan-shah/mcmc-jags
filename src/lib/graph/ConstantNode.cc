#include <config.h>
#include <graph/ConstantNode.h>
#include <graph/NodeNameTab.h>

#include <sstream>
#include <cmath>

using std::vector;
using std::string;
using std::ostringstream;
using std::floor;

ConstantNode::ConstantNode(double value, unsigned int nchain)
  : Node(vector<unsigned int>(1,1), nchain)
{
  setObserved(&value, 1);
  //FIXME: This should be done within the setObserved function
  if (value == floor(value)) {
      setDiscreteValued();
  }
}

void ConstantNode::deterministicSample(unsigned int) {};

void ConstantNode::randomSample(RNG*, unsigned int) {};

string ConstantNode::name(NodeNameTab const &name_table) const
{
  string name = name_table.getName(this);
  if (!name.empty())
    return name;

  ostringstream os;
  os << *value(0);
  return os.str();
}
