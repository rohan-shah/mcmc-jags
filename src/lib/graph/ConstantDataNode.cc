#include <config.h>
#include <graph/ConstantDataNode.h>

using std::vector;

namespace jags {

    ConstantDataNode::ConstantDataNode(double value, unsigned int nchain)
	: ConstantNode(value, nchain)
    {
    }

    ConstantDataNode::ConstantDataNode(vector<unsigned int> const &dim, 
				       vector<double> const &value,
				       unsigned int nchain)
	: ConstantNode(dim, value, nchain)
    {
    }

    RVStatus ConstantDataNode::randomVariableStatus() const
    {
	return RV_TRUE_OBSERVED;
    }

} //namespace jags
