#ifndef SHIFTED_POISSON_H_
#define SHIFTED_POISSON_H_

#include "ConjugateMethod.h"

namespace bugs {

    class ShiftedPoisson : public ConjugateMethod {
    public:
	ShiftedPoisson(GraphView const *gv);
	static bool canSample(StochasticNode *snode, Graph const &graph);
	void update(unsigned int chain, RNG *rng) const;
	std::string name() const;
    };

}

#endif /* SHIFTED_POISSON_H_ */
