#ifndef SHIFTED_COUNT_H_
#define SHIFTED_COUNT_H_

#include "ConjugateMethod.h"

namespace bugs {

    class ShiftedCount : public ConjugateMethod {
    public:
	ShiftedCount(GraphView const *gv);
	static bool canSample(StochasticNode *snode, Graph const &graph);
	void update(unsigned int chain, RNG *rng) const;
	std::string name() const;
    };

}

#endif /* SHIFTED_COUNT_H_ */
