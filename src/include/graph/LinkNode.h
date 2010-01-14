#ifndef LINK__NODE_H_
#define LINK__NODE_H_

#include <graph/LogicalNode.h>
#include <vector>

class InverseLinkFunc;

/**
 * @short Scalar Node representing link function from a GLM 
 */
class LinkNode : public LogicalNode {
    InverseLinkFunc const * const _func;
public:
    /**
     * A link node is defined by an inverse link function and a single
     * parent node, which must be scalar
     */
    LinkNode(InverseLinkFunc const *func, 
	     std::vector<Node const *> const &parents);
    /**
     * Returns the link function
     */
    InverseLinkFunc const * link() const;
    /**
     * Calculates the value of the node based on the parameters. 
     */
    void deterministicSample(unsigned int chain);
    /**
     * Returns true. An inverse link function should accept every
     * value in the range [-Inf, Inf].
     */
    bool checkParentValues(unsigned int chain) const;
    /**
     * Returns the linear predictor
     */
    double eta(unsigned int chain) const;
    /**
     * Returns the gradient of the inverse link function
     * @see InverseLinkFunc#grad
     */
    double grad(unsigned int chain) const;
    Node *clone(std::vector<Node const *> const &parents) const;
};

#endif /* LINK_NODE_H_ */
