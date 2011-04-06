#ifndef SCALAR_DIST_H_
#define SCALAR_DIST_H_

#include <distribution/Distribution.h>

struct RNG;

/**
 * Enumerates three possible ranges of support for a scalar random
 * variable.
 *
 * DIST_UNBOUNDED for support on the whole real line
 *
 * DIST_POSITIVE for support on values > 0
 *
 * DIST_PROPORTION for support on values in [0,1]
 *
 * DIST_SPECIAL for other distributions, e.g. distributions where the
 * support depends on the parameter values.
 */
enum Support {DIST_UNBOUNDED, DIST_POSITIVE, DIST_PROPORTION, DIST_SPECIAL};

/**
 * @short Scalar distributions
 *
 * The ScalarDist class represents distributions that are scalar-valued
 * and for which the parameters are also scalars.
 */
class ScalarDist : public Distribution
{
  const Support _support;
 public:
  /**
   * Constructor
   *
   * @param name BUGS language name of distribution
   *
   * @param npar Number of parameters, excluding upper and lower bound
   *
   * @param support Support of distribution
   *
   * @param discrete Logical flag indicating whether the distribution
   * is discrete-valued.
   */
  ScalarDist(std::string const &name, unsigned int npar, Support support);
  /**
   * This implementation should be used for distributions with
   * Support DIST_UNBOUNDED, DIST_POSITIVE and DIST_PROPORTION. If
   * the Support is DIST_SPECIAL, this must be overloaded.
   */
  bool isSupportFixed(std::vector<bool> const &fixmask) const;
  /**
   * Lower limit of distribution, given parameters.  If the
   * distribution has no lower limit, this should return JAGS_NEGINF.
   *
   * The default implementation should be used for distributions with
   * Support DIST_UNBOUNDED, DIST_POSITIVE and DIST_PROPORTION. If
   * the Support is DIST_SPECIAL, this must be overloaded.
   */
  virtual double l(std::vector<double const *> const &parameters) const;
  /**
   * Upper limit of distribution, given parameters. If the
   * distribution has no upper limit, this should return JAGS_POSINF.
   *
   * The default implementation should be used for distributions with
   * Support DIST_UNBOUNDED, DIST_POSITIVE and DIST_PROPORTION. If
   * the Support is DIST_SPECIAL, this must be overloaded.
   */
  virtual double u(std::vector<double const *> const &parameters) const;
  /**
   * Calculates log probability density of the distribution
   */
  virtual double logDensity(double x, PDFType type,
			    std::vector<double const *> const &parameters,
			    double const *lbound, double const *ubound)
      const = 0;
  /**
   * Draws a random sample 
   */
  virtual double randomSample(std::vector<double const *> const &parameters, 
			      double const *lbound, double const *ubound,
			      RNG *rng) const = 0;
  /**
   * Generate a deterministic value with reasonably high density
   * (e.g. mean, median, mode ...)
   */
  virtual double typicalValue(std::vector<double const *> const &parameters,
			      double const *lbound, double const *ubound)
      const = 0;
  /**
   * Checks whether the parameter values are valid
   */
  virtual bool checkParameterValue(std::vector<double const *> const &params) 
      const = 0;
  /**
   * Returns the number of degrees of freedom of the distribution.
   * By default this is 1. For scalar distributions that are
   * deterministic functions of the parameters, this must be overridden.
   */
  virtual unsigned int df() const;
  /*
  virtual double KL(std::vector<double const *> const &par1,
		    std::vector<double const *> const &par2,
		    bool symmetric, RNG *rng, unsigned int nrep);
  */
};

#endif /* SCALAR_DIST_H_ */
