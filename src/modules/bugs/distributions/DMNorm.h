#ifndef DMNORM_H_
#define DMNORM_H_

#include <distribution/Distribution.h>

/**
 * @short Multivariate normal distribution
 * <pre>
 * x[] ~ dmnorm(mu[], T[,])
 * f(x | mu, T) = sqrt(det(T)) * exp(-1/2 * (x-mu) %*% T %*% t(x-mu))
 * </pre>
 */
class DMNorm : public Distribution {
public:
  DMNorm();

  double logLikelihood(double const *x, unsigned int length,
		       std::vector<double const *> const &parameters,
		       std::vector<std::vector<unsigned int> > const &dims)
      const;
  void randomSample(double *x, unsigned int length,
		    std::vector<double const *> const &parameters,
		    std::vector<std::vector<unsigned int> > const &dims,
		    RNG *rng) const;
  /**
   * Checks that mu is a vector, T is a square matrix and the sizes of
   * mu and T conform.
   */
  bool checkParameterDim(std::vector<std::vector<unsigned int> > const &dims) 
      const;
  /**
   * Checks that T is symmetric. Note that there is currently NO CHECK
   * that T is positive definite.
   */
  bool checkParameterValue(std::vector<double const *> const &parameters,
                           std::vector<std::vector<unsigned int> > const &dims) 
      const;
  std::vector<unsigned int> 
      dim(std::vector<std::vector<unsigned int> > const &dims) const;
  /**
   * Convenience random sampler function 
   */
  static void randomsample(double *x, double const *mu, double const *tau,
			   int nrow, RNG *rng);
  void support(double *lower, double *upper, unsigned int length,
	       std::vector<double const *> const &parameters,
               std::vector<std::vector<unsigned int> > const &dims) const;
  void typicalValue(double *x, unsigned int length, 
		    std::vector<double const *> const &parameters,
               std::vector<std::vector<unsigned int> > const &dims) const;
  bool isSupportFixed(std::vector<bool> const &fixmask) const;
};

#endif /* DMNORM_H_ */
