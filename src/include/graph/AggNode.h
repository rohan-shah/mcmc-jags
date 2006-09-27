#ifndef AGGREGATE_NODE_H_
#define AGGREGATE_NODE_H_

#include <graph/DeterministicNode.h>

/**
 * An aggregate Node copies its values directly from its parents: it can
 * be used to aggregate several small nodes into a larger one, or to take
 * a subset of a larger node, or some combination of the two.
 *
 * @short Aggregate Node
 */
class AggNode : public DeterministicNode {
    std::vector<unsigned int> _offsets;
    /* Forbid copying */
    AggNode(AggNode const &orig);
    AggNode &operator=(AggNode const &rhs);
public:
    /**
     * Constructor.  
     *
     * @param dim Dimension of the Node.
     *
     * @param nodes Vector of parent Nodes. Each element corresponds
     * to an element of the value vector of the AggNode to be
     * constructed, and gives the source of the value to be copied.
     * A node may appear several times in this vector.
     *
     * @param offsets Vector of offsets of the same length as the
     * nodes vector. The offset indicates which element of the value
     * vector of the corresponding parent should be taken.
     */
    AggNode(std::vector<unsigned int> const &dim,
	    std::vector<Node*> const &nodes, 
	    std::vector<unsigned int> const &offsets);
    ~AggNode();
    /**
     * Copies values from parents.
     */
    void deterministicSample(unsigned int chain);
    /**
     * An AggNode always preserves linearity.
     */
    bool isLinear(std::set<Node*> const &parameters, bool fixed) const;
    /**
     * An AggNode is a scale transformation only if it has a single
     * parent, and in this case it is also fixed.
     */
    bool isScale(std::set<Node*> const &parameters, bool fixed) const;
};

#endif /* AGGREGATE_NODE_H */
