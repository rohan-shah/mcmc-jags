#ifndef STOCHASTIC_NODE_H_
#define STOCHASTIC_NODE_H_

#include <vector>

#include <graph/Node.h>

class Distribution;
class RNG;

class StochasticNode : public Node {
    Distribution const * const _dist;
    std::vector<std::vector<SArray const *> > _parameters;
    Node *_lower, *_upper;

    /* Forbid copying of Stochastic Nodes */
    StochasticNode(StochasticNode const &orig);
    StochasticNode &operator=(StochasticNode const &rhs);
 public:

    StochasticNode(Distribution const *dist, 
                   std::vector<Node*> const &parameters,
                   Node *lower, Node *upper);
    ~StochasticNode();

    bool isBounded() const;
    Node const *lowerBound();
    Node const *upperBound();
    Distribution const *distribution() const;
    std::vector<SArray const *> const &parameters(unsigned int chain) const;
    double logDensity(unsigned int chain) const;
    void randomSample(RNG *rng, unsigned int chain);
    void deterministicSample(unsigned int chain);
    bool isVariable() const;
};

StochasticNode const *asStochastic(Node const *node);

#endif /* STOCHASTIC_NODE_H_ */
