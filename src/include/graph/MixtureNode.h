#ifndef MIXTURE_NODE_H_
#define MIXTURE_NODE_H_

#include <graph/DeterministicNode.h>
#include <map>
#include <vector>

/**
 * A mixture node copies its value from one of several parents, based
 * on the current values of a set of index nodes, which are also
 * parents.  
 *
 * @short Node for mixture models.
 */
class MixtureNode : public DeterministicNode {
    std::map<std::vector<int>, Node const *> _map;
    unsigned int _Nindex;
public:
    /**
     * Constructs a MixtureNode. 
     * @param index Vector of discrete-valued scalar nodes
     * @param parameters Vector of pairs. Each pair associates a
     * possible value of the index nodes with a parent. The mixture node
     * copies its value from the parent that matches the current value
     * of the index.
     */
    MixtureNode(std::vector<Node const *> const &index,
		std::vector<std::pair<std::vector<int>, Node const *> > const &parameters);
    ~MixtureNode();
    /**
     * Calculates the value of the node based on the current value of the
     * index and the value of the corresponding parameter.
     */
    void deterministicSample(unsigned int chain);
    /**
     * Returns the number of index parameters.
     */
    unsigned int index_size() const;
    /**
     * A MixtureNode preserves linearity if none of the indices are
     * parameters. It is never a fixed linear function.
     */
    bool isLinear(std::set<Node const*> const &parameters, bool fixed) const;
    /**
     * A MixtureNode is a scale transformation if none of the indices
     * are parameters. It is never a fixed scale transformation.
     */
    bool isScale(std::set<Node const*> const &parameters, bool fixed) const;
};

MixtureNode const *asMixture(Node const *node);
bool isMixture(Node const *node);

#endif /* MIXTURE_NODE_H_ */
