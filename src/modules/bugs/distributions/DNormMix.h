#ifndef DNORM_MIX_H_
#define DNORM_MIX_H_

#include <distribution/Distribution.h>

/**
 * <pre>
 * x ~ dnorm(mu, tau, p)
 * </pre>
 * @short Mixture of normal distribution
 */
class DNormMix : public Distribution {
 public:
    DNormMix();

    double logLikelihood(double const *x, unsigned int length,
			 std::vector<double const *> const &parameters,
			 std::vector<std::vector<unsigned int> > const &dims)
	const;
    void randomSample(double *x, unsigned int length,
		      std::vector<double const *> const &parameters,
		      std::vector<std::vector<unsigned int> > const &dims,
		      RNG *rng) const;
    std::vector<unsigned int> 
	dim(std::vector<std::vector<unsigned int> > const &dims) const;
    void support(double *lower, double *upper, unsigned int length,
		 std::vector<double const *> const &parameters,
		 std::vector<std::vector<unsigned int> > const &dims) const;
    void typicalValue(double *x, unsigned int length,
		      std::vector<double const *> const &parameters,
		      std::vector<std::vector<unsigned int> > 
		      const &dims) const;
    bool isSupportFixed(std::vector<bool> const &fixmask) const;
    bool checkParameterDim(std::vector<std::vector<unsigned int> > const &dims)
	const;
    bool checkParameterValue(std::vector<double const *> const &parameters,
			     std::vector<std::vector<unsigned int> > 
			     const &dims) const;
    void typicalValue(double *x, std::vector<double const *> const &par,
		      std::vector<std::vector<unsigned int> > const &dims) 
	const;
};

#endif /* DNORM_MIX_H_ */
