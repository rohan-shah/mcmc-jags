#ifndef TRUNCATED_GAMMA_H_
#define TRUNCATED_GAMMA_H_

#include "ConjugateMethod.h"

class Node;

namespace bugs {

class TruncatedGamma : public ConjugateMethod {
    double _shape;
    Node const *_parent;
public:
    TruncatedGamma(GraphView const *gv);
    ~TruncatedGamma();
    static bool canSample(StochasticNode *snode, Graph const &graph);
    void update(unsigned int chain, RNG *rng) const;
    std::string name() const;
};

}

#endif /* TRUNCATED_GAMMA_H_ */
