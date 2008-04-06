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
     * Returns false. Link nodes are non-linear.
     */
    bool isLinear(GraphMarks const &linear_marks, bool fixed) const;
    /**
     * Returns false. Link nodes are not scale functions.
     */
    bool isScale(GraphMarks const &scale_marks, bool fixed) const;
    std::string deparse(std::vector<std::string> const &) const;
    Node *clone(std::vector<Node const *> const &parents) const;
};

#endif /* LINK_NODE_H_ */
