#include <config.h>
#include <graph/ConstantNode.h>
#include <graph/NodeNameTab.h>

#include <sstream>
#include <cmath>
#include <algorithm>

using std::vector;
using std::string;
using std::ostringstream;
using std::floor;
using std::copy;

ConstantNode::ConstantNode(double value, unsigned int nchain)
  : Node(vector<unsigned int>(1,1), nchain)
{
    setObserved(&value, 1);
    //FIXME: This should be done within the setObserved function
    if (value == floor(value)) {
	setDiscreteValued();
    }
}

ConstantNode::ConstantNode(vector<unsigned int> const &dim, 
			   vector<double> const &value,
			   unsigned int nchain)
    : Node(dim, nchain)
{
    double *v = new double[value.size()];
    copy(value.begin(),value.end(),v);
    setObserved(v, value.size());
    delete [] v;
}

void ConstantNode::deterministicSample(unsigned int) {}

void ConstantNode::randomSample(RNG*, unsigned int) {}

string ConstantNode::name(NodeNameTab const &name_table) const
{
  string name = name_table.getName(this);
  if (!name.empty())
    return name;

  ostringstream os;
  os << *value(0);
  return os.str();
}

bool ConstantNode::isLinear(GraphMarks const &linear_marks, bool fixed) const
{
    return true;
}

bool ConstantNode::isScale(GraphMarks const &scale_marks, bool fixed) const
{
    return true;
}

bool ConstantNode::checkParentValues(unsigned int) const
{
    return true;
}

bool ConstantNode::isRandomVariable() const
{
    return true;
}

