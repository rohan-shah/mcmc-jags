#include <config.h>
#include <graph/ConstantNode.h>

#include <sstream>
#include <cmath>
#include <algorithm>
#include <stdexcept>

using std::vector;
using std::string;
using std::ostringstream;
using std::floor;
using std::copy;
using std::logic_error;

ConstantNode::ConstantNode(double value, unsigned int nchain)
  : Node(vector<unsigned int>(1,1), nchain)
{
    for (unsigned int n = 0; n < nchain; ++n) {
       setValue(&value, 1, n);
    }
}

ConstantNode::ConstantNode(vector<unsigned int> const &dim, 
			   vector<double> const &value,
			   unsigned int nchain)
    : Node(dim, nchain)
{
    if (value.size() != _length) {
        throw logic_error("Invalid value in ConstantNode");
    }

    //FIXME: Should we be using an array in constructor anyway?
    double *val = new double[_length];
    copy(value.begin(), value.end(), val);
    for (unsigned int n = 0; n < nchain; ++n) {
       setValue(val, _length, n);
    }
    delete [] val;
}

ConstantNode::~ConstantNode()
{
}

bool ConstantNode::isDiscreteValued() const
{
    double const *val = value(0);
    for (unsigned int i = 0; i < _length; ++i) {
        if (val[i] != floor(val[i]))
            return false;
    }
    return true;
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

bool ConstantNode::checkParentValues(unsigned int) const
{
    return true;
}

bool ConstantNode::isRandomVariable() const
{
    return true;
}

bool ConstantNode::isObserved() const
{
    return true;
}
