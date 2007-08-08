#ifndef DISCRETE_SLICE_SAMPLER_H_
#define DISCRETE_SLICE_SAMPLER_H_

#include <sampler/Slicer.h>

class StochasticNode;
struct RNG;

namespace basesamplers {

/**
 * @short Discrete slice sampler
 *
 * Slice sampler for discrete valued distributions.
 *
 * To sample a discrete valued random variable X, we create an
 * auxiliary variable Y which is real valued, and for which p_[Y](y) =
 * p_[X](floor(y)).  Hence Y has a piecewise-constant density function
 * on the real line.  We sample Y and then set X = floor(Y).
 */
class DiscreteSliceSampler : public Slicer {
  const unsigned int _chain;
  double _x;
 public:
  /**
   * Constructor for Slice Sampler
   * @param node Node to sample
   * @param graph Graph within which sampling takes place
   * @param width Initial width of slice
   * @param ndoubles Maximum number of doublings of slice 
   */
  DiscreteSliceSampler(StochasticNode *node, Graph const &graph,
		       unsigned int chain, double width=2, long ndoubles = 10);
  void setValue(double x);
  double value() const;
  static bool canSample(StochasticNode const *node, Graph const &graph);
  void getLimits(double *lower, double *upper) const;
  void update(RNG*);
  double logDensity() const;
};

}

#endif /* DISCRETE_SLICE_SAMPLER_H_ */
