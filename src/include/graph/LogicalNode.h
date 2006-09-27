#ifndef LOGICAL_NODE_H_
#define LOGICAL_NODE_H_

#include <graph/DeterministicNode.h>
#include <vector>

class Function;
class SArray;

/**
 * A LogicalNode is a type of DeterministicNode that can be defined by
 * a logical relation in the BUGS language.
 *
 * @short Logical Node
 */
class LogicalNode : public DeterministicNode {
    Function const * const _func;
    std::vector<std::vector<SArray const*> > _parameters;

    /* Forbid copying of logical nodes */
    LogicalNode(LogicalNode const &orig);
    LogicalNode &operator=(LogicalNode const &rhs);
public:
    /**
     * A logical node is defined by a function (which may be an inline
     * operator in the BUGS language) and its parameters.
     */
    LogicalNode(Function const *func, std::vector<Node*> const &parameters);
    ~LogicalNode();
    /** 
     * Returns the parameters of the node as a vector of SArray
     * pointers
     */
    std::vector<SArray const *> const &parameters(unsigned int chain) const;
    /** 
     * Returns the Function used to calculate the value of the logical
     * node.
     */
    Function const* function() const;
    /** 
     * Calculates the value of the node based on the parameters. This
     * function calls Function##checkParameterValue and will throw an
     * exception if the result is false.
     * @exception NodeError
     */
    void deterministicSample(unsigned int chain);
    /**
     * If a LogicalNode does not have a name defined in the name
     * table, then its name is constructed to be the BUGS-language
     * representation, i.e. the right-hand side of a logical relation
     * that defines the node.
     */
    std::string name(NodeNameTab const &name_table) const;
    /**
     * A logical node preserves linearity if its function is linear.
     * @see Function##isLinear
     */
    bool isLinear(std::set<Node*> const &parameters, bool fixed) const;
    /**
     * A logical node preserves scale if its function is a scale function.
     * @see Function##isScale
     */
    bool isScale(std::set<Node*> const &parameters, bool fixed) const;
};

bool isLogical(Node const *node);
LogicalNode const *asLogical(Node const *node);

#endif /* LOGICAL_NODE_H_ */
