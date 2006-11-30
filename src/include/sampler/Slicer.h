#ifndef SLICER_H_
#define SLICER_H_

#include <sampler/Sampler.h>

class StochasticNode;

/**
 * Base class for univariate slice sampler (Neal R, Slice Sampling,
 * Ann. Statist.  31 (2003), no. 3, 705-767)
 *
 * The Slicer class is written with a high level of abstraction to
 * allow it to be easily adapted: for example, a slice sampler can be
 * used to sample from a discrete distribution or from a univariate
 * "slice" of a multivariate distribution.
 */
class Slicer : public Sampler 
{
  double _width;
  bool _adapt;
  unsigned int _max;
  double _sumdiff;
  unsigned int _iter;
  bool accept(double xold, double xnew, double z, double L, double R);
 public:
  /**
   * Constructor for Slice Sampler
   * @param node Node to sample
   * @param Graph containing node within which sampling takes place.
   * @param width Initial width of slice
   * @param max Maximum number of times initial width of slice will increase.
   */
  Slicer(std::vector<StochasticNode*> const &nodes, Graph const &graph,
	 double width,  unsigned int max);
  /**
   * Update the current value using the "stepping" method
   */
  void updateStep(RNG *rng);
  /**
   * Update the current value using the "doubling" method
   */
  void updateDouble(RNG *rng);
  /**
   * Returns the current value of the sampler
   */
  virtual double value() const = 0;
  /**
   * Sets the value of the sampler, and additionally updates all the
   * deterministic descendants
   */
  virtual void setValue(double x) = 0;
  /**
   * Gets the lowest and highest possible values of the sampler
   */
  virtual void getLimits(double *lower, double *upper) const = 0;
  void adaptOff();
  virtual double logFullConditional() const = 0;
};

#endif /* SLICER_H_ */
