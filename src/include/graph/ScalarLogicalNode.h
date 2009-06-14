#ifndef SCALAR_LOGICAL_NODE_H_
#define SCALAR_LOGICAL_NODE_H_

#include <graph/DeterministicNode.h>
#include <vector>

class ScalarFunc;

/**
 * @short Scalar Node defined by the BUGS-language operator <-
 *
 * A ScalarLogicalNode is a special case of a logical node that 
 * uses a ScalarFunc as the defining function and is not vectorized.
 *
 * It is possible to construct a standard LogicalNode with exactly
 * the same arguments, which will behave in the same way. However,
 * the ScalarLogicalNode is more efficient as it stores less auxilliary
 * data and removes one level of indirection from the function calls.
 *
 */
class ScalarLogicalNode : public DeterministicNode {
    ScalarFunc const * const _func;
    std::vector<std::vector<double const*> > _parameters;
public:
    /**
     * A scalar logical node is defined by a scalar function (which
     * may be an inline operator in the BUGS language) and its
     * parameters.
     */
    ScalarLogicalNode(ScalarFunc const *func,
		      std::vector<Node const*> const &parameters);
    /**
     * Calculates the value of the node based on the parameters. 
     */
    void deterministicSample(unsigned int chain);
    /**
     * @see Function#checkParameterValue.
     */
    bool checkParentValues(unsigned int chain) const;
    /**
     * A ScalarLogicalNode belongs to a closed class if its function
     * preserves that class.
     *
     * @see ScalarFunc#isLinear, ScalarFunc#isScale, ScalarFunc#isPower
     */
    bool isClosed(std::set<Node const *> const &ancestors, 
		  ClosedFuncClass fc, bool fixed) const;
    std::string deparse(std::vector<std::string> const &) const;
    Node *clone(std::vector<Node const *> const &parents) const;
    bool isDiscreteValued() const;
};

#endif /* SCALAR_LOGICAL_NODE_H_ */
