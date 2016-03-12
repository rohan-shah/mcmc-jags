#ifndef BINARY_LOGIT_H_
#define BINARY_LOGIT_H_

#include <config.h>

#include "Outcome.h"
#include <graph/StochasticNode.h>

namespace jags {

class StochasticNode;

namespace glm {

    class BinaryLogit : public Outcome
    {
	double const &_y;
	double _z;
	double _tau;
	double _sigma2;
      public:
	BinaryLogit(StochasticNode const *snode, unsigned int chain);
	double value() const;
	double precision() const;
	void update(RNG *rng);
	void update(double mean, double var, RNG *rng);
	static bool canRepresent(StochasticNode const *snode);
    };

}}

#endif /* BINARY_LOGIT_H_ */
