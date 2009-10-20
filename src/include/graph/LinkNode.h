#ifndef LINK__NODE_H_
#define LINK__NODE_H_

#include <graph/DeterministicNode.h>
#include <vector>

class InverseLinkFunc;

/**
 * @short Scalar Node representing link function from a GLM 
 */
class LinkNode : public DeterministicNode {
    InverseLinkFunc const * const _func;
    double const * _parameters;
public:
    /**
     * A link node is defined by an inverse link function and a single
     * parent node, which must be scalar
     */
    LinkNode(InverseLinkFunc const *func,
             std::vector<Node const *> const &parameters);
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
     * @see InverseLinkFunc##grad
     */
    double grad(unsigned int chain) const;
    /**
     * Link nodes are not closed
     */
    bool isClosed(std::set<Node const *> const &ancestors, 
		  ClosedFuncClass fc, bool fixed) const;
    std::string deparse(std::vector<std::string> const &) const;
    Node *clone(std::vector<Node const *> const &parents) const;
    /** Linknodes are never discrete valued */
    bool isDiscreteValued() const;
};

#endif /* LINK_NODE_H_ */
