#ifndef DIST_SCALAR_RMATH_H_
#define DIST_SCALAR_RMATH_H_

#include <distribution/DistScalar.h>

struct RNG;

/**
 * Base class for scalar valued distributions.
 *
 * A subclass of DistScalar has to implement the d,p,q, and r virtual
 * member functions. These are based on the d-p-q-r functions provided
 * by libRmath.
 *
 * The JAGS versions of most (but not all) scalar distributions extend
 * the distribution families in libRmath by allowing the distribution
 * to be bounded.
 *
 * @short Real valued distributions
 */
class DistScalarRmath : public DistScalar
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
   * @param boundable Logical flag indicating whether the distribution
   * can be bounded using the T(,) construct.
   *
   * @param discrete Logical flag indicating whether the distribution
   * is discrete-valued.
   */
  DistScalarRmath(std::string const &name, unsigned int npar,
	     Support support, bool canbound, bool discrete);
  double logLikelihood(double x,
                       std::vector<double const *> const &parameters) const;
  double randomSample(std::vector<double const *> const &parameters,
                      RNG *rng) const;
  /**
   * Returns the median. Note that this function can be overloaded
   * by a subclass if necessary.
   */
  double typicalValue(std::vector<double const *> const &parameters) const;
  /** 
   * Density function, ignoring bounds
   * @param x value at which to evaluate the density
   * @param parameters Array of parameters
   * @param give_log Indicates whether to return log density. 
   */
  virtual double d(double x, std::vector<double const *> const &parameters, 
		   bool give_log) const = 0;
  /** 
   * Distribution function, ignoring bounds
   * @param x quantile at which to evaluate the distribution function
   * @param parameters Array of parameters
   * @param lower If true, return value is P[X <= x]. Otherwise
   * P[X > x]
   * @param give_log Indicates whether to return log probabability
   */
  virtual double p(double x, std::vector<double const *> const &parameters, 
		   bool lower, bool give_log) const = 0;
  /** 
   * Quantile function, ignoring bounds
   * @param p probability for which to evaluate quantile
   * @param parameters Array of parameters
   * @param log_p Indicates whether p is given as log(p). 
   */
  virtual double q(double p, std::vector<double const *> const &parameters, 
		   bool lower, bool log_p) const = 0;
  /**
   * Random number generation, ignoring bounds
   * @param parameters Array of parameters
   */
  virtual double 
    r(std::vector<double const *> const &parameters, RNG *rng) const = 0;
};

#endif /* DIST_SCALAR_RMATH_H_ */
