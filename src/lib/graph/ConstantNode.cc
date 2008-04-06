#include <config.h>
#include <graph/ConstantNode.h>

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
    setObserved(vector<double>(1,value));
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
    setObserved(value);
}

void ConstantNode::deterministicSample(unsigned int) {}

void ConstantNode::randomSample(RNG*, unsigned int) {}

string ConstantNode::deparse(vector<string> const &parents) const
{
    ostringstream os;
    if (length() == 1) {
       os << *value(0);
    }
    else {
       os << "c(" << value(0)[0] << "..."  << value(0)[length() - 1] << ")";
    }
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

Node *ConstantNode::clone(vector<Node const *> const &parents) const
{
    vector<double> value(this->length());
    for (unsigned int i = 0; i < this->length(); ++i) {
	value[i] = this->value(0)[i];
    }
    return new ConstantNode(this->dim(), value, this->nchain());
}

