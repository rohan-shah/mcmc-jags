#ifndef CONSTANT_NODE_H_
#define CONSTANT_NODE_H_

#include <graph/Node.h>

namespace jags {

/**
 * @short Top-level Node with constant value
 *
 * Constant nodes are the top-level nodes in any directed acyclic
 * graph (i.e. they have no parents). They have a fixed value that is
 * defined when they are constructed and which is shared across all
 * chains.
 *
 * There are two sub-classes of ConstantNode representing explicit
 * constants defined in the model (ConstantParameterNode) and implicit
 * constants with values supplied by the data (ConstantDataNode).
 */
class ConstantNode : public Node {
public:
    /**
     * Constructs a scalar constant node and sets its value. The value
     * is fixed and is shared between all chains.
     */
    ConstantNode(double value, unsigned int nchain);
    /**
     * Constructs a multi-dimensional constant node 
     */
    ConstantNode(std::vector<unsigned int> const &dim, 
		 std::vector<double> const &value,
		 unsigned int nchain);
    /**
     * Indicates whether a ConstantNode is discrete-valued
     */
    bool isDiscreteValued() const;
    /**
     * This function does nothing. It exists only so that objects
     * inheriting from ConstantNode can be instantiated.
     */
    void deterministicSample(unsigned int);
    /**
     * This function does nothing. The value of the constant node is
     * not changed and the state of the RNG remains the same.
     */
    void randomSample(RNG*, unsigned int);
    /**
     * Constant nodes have no parents. This function always returns true.
     */
    bool checkParentValues(unsigned int) const;
    /**
     * A constant node is named after its value
     */
    std::string deparse(std::vector<std::string> const &parents) const;
    /**
     * A constant node is always fixed.
     */
    bool isFixed() const;

};

} /* namespace jags */

#endif /* CONSTANT_NODE_H_ */




