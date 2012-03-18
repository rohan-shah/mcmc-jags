#ifndef SHIFTED_MULTINOMIAL_H_
#define SHIFTED_MULTINOMIAL_H_

#include "ConjugateMethod.h"

namespace bugs {

    class ShiftedMultinomial : public ConjugateMethod {
	std::vector<int> _index;
    public:
	ShiftedMultinomial(GraphView const *gv);
	static bool canSample(StochasticNode *snode, Graph const &graph);
	void update(unsigned int chain, RNG *rng) const;
	std::string name() const;
    };

}

#endif /* SHIFTED_MULTINOMIAL_H_ */
