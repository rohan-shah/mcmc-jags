#ifndef SHIFTED_COUNT_H_
#define SHIFTED_COUNT_H_

#include "ConjugateMethod.h"

namespace jags {

    class Graph;

namespace bugs {

    class ShiftedCount : public ConjugateMethod {
    public:
	ShiftedCount(SingletonGraphView const *gv);
	static bool canSample(StochasticNode *snode, Graph const &graph);
	void update(unsigned int chain, RNG *rng) const;
    };

}}

#endif /* SHIFTED_COUNT_H_ */
