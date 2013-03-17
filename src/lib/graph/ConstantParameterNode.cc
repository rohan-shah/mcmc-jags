#include <config.h>
#include <graph/ConstantParameterNode.h>

using std::vector;

namespace jags {

    ConstantParameterNode::ConstantParameterNode(double value, 
						 unsigned int nchain)
	: ConstantNode(value, nchain)
    {
    }
    
    ConstantParameterNode::ConstantParameterNode(vector<unsigned int> const &d, 
						 vector<double> const &value,
						 unsigned int nchain)
	: ConstantNode(d, value, nchain)
    {
    }

    RVStatus ConstantParameterNode::randomVariableStatus() const
    {
	return RV_FALSE;
    }

} //namespace jags
