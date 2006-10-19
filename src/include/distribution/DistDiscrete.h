#ifndef DIST_DISCRETE_H_
#define DIST_DISCRETE_H_

#include <distribution/DistScalarRmath.h>

/**
 * Base class for integer valued distributions
 *
 * @see DistReal
 * @short Integer valued distributions
 */
class DistDiscrete : public DistScalarRmath
{
public:
  /**
   * Constructor for discrete distribution
   *
   * @param name BUGS language name of the distribution
   * @param npar Number of parameters, excluding upper and lower bounds
   * @param support Support of the distribution.
   * @param canbound Logical flag indicating whether distribution is boundable.
   */
  DistDiscrete(std::string const &name, unsigned int npar, 
	       Support support, bool canbound);
  double logLikelihood(double x,
		       std::vector<double const *> const &parameters) const;
  double randomSample(std::vector<double const *> const &parameters, RNG *rng) 
      const;
};

#endif /* DIST_DISCRETE_H_ */
