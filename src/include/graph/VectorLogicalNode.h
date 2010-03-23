#ifndef VECTOR_LOGICAL_NODE_H_
#define VECTOR_LOGICAL_NODE_H_

#include <graph/LogicalNode.h>

#include <vector>

class VectorFunction;

/**
 * @short Vector-valued Node defined by the BUGS-language operator <-
 *
 * A LogicalNode is a type of DeterministicNode that is defined by a
 * function, and a vector of parents as parameters. In the BUGS
 * language the function may be represented as an operator.
 */
class VectorLogicalNode : public LogicalNode {
    VectorFunction const * const _func;
    std::vector<unsigned int> _lengths;
public:
    /**
     * A logical node is defined by a function (which may be an inline
     * operator in the BUGS language) and its parameters.
     */
    VectorLogicalNode(VectorFunction const *func,
		      std::vector<Node const*> const &parameters);
    /**
     * Calculates the value of the node based on the parameters. 
     */
    void deterministicSample(unsigned int chain);
    /**
     * @see VectorFunction#checkParameterValue.
     */
    bool checkParentValues(unsigned int chain) const;
    DeterministicNode *clone(std::vector<Node const *> const &parents) const;
};

#endif /* VECTOR_LOGICAL_NODE_H_ */
