#ifndef CONSTANT_DATA_NODE_H_
#define CONSTANT_DATA_NODE_H_

#include <graph/ConstantNode.h>

namespace jags {

/**
 * @short Top-level Node representing data
 *
 * Constant data nodes are implicit constants that appear only on the
 * right hand side of a relation, and whose values are defined by
 * user-supplied data. They are considered to represent observed
 * random variables.
 */
class ConstantDataNode : public ConstantNode {
public:
    /**
     * Constructs a scalar constant node and sets its value. The value
     * is fixed and is shared between all chains.
     */
    ConstantDataNode(double value, unsigned int nchain);
    /**
     * Constructs a multi-dimensional constant node
     */
    ConstantDataNode(std::vector<unsigned int> const &dim, 
		 std::vector<double> const &value,
		 unsigned int nchain);
    /**
     * A ConstantDataNode is an observed random variable.
     */
    RVStatus randomVariableStatus() const;
};

} /* namespace jags */

#endif /* CONSTANT_DATA_NODE_H_ */




