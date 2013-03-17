#ifndef CONSTANT_PARAMETER_NODE_H_
#define CONSTANT_PARAMETER_NODE_H_

#include <graph/ConstantNode.h>

namespace jags {

/**
 * @short Top-level Node representing a constant defined in the model
 *
 * A Constant parameter node is a node that is defined in the model to
 * have fixed value.  This is typically used in linear models to
 * generate contrasts.
 *
 * <pre>
 * beta[1] <- 0 #This is a constant parameter node
 * for (i in 2:N) {
 *     beta[i] ~ dnorm(0, 1) #These are contrast parameters
 * }
 * </pre>
 */
class ConstantParameterNode : public ConstantNode {
public:
    /**
     * Constructs a constant node and sets its value. The value is
     * fixed and is shared between all chains.
     */
    ConstantParameterNode(double value, unsigned int nchain);
    /**
     * Constructs a multi-dimensional constant node
     */
    ConstantParameterNode(std::vector<unsigned int> const &dim, 
			  std::vector<double> const &value,
			  unsigned int nchain);
    /**
     * A ConstantParameterNode is not a random variable.
     */
    RVStatus randomVariableStatus() const;
};

} /* namespace jags */

#endif /* CONSTANT_PARAMETER_NODE_H_ */




