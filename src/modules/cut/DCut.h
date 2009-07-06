#ifndef DCUT_H_
#define DCUT_H_

#include <distribution/Distribution.h>

/**
 * @short Cut observable function
 * <pre>
 * i ~ dcut(t)
 * </pre>
 */
class DCut : public Distribution {

public:
    DCut();
  
    double logLikelihood(double const *x, unsigned int length,
			 std::vector<double const *> const &parameters,
			 std::vector<std::vector<unsigned int> > const &dims,
			 double const *lower, double const *upper) const;
    void randomSample(double *x, unsigned int length,
		      std::vector<double const *> const &parameters,
		      std::vector<std::vector<unsigned int> > const &dims,
		      double const *lower, double const *upper, RNG *rng) const;
    void typicalValue(double *x, unsigned int length,
		      std::vector<double const *> const &parameters,
		      std::vector<std::vector<unsigned int> > const &dims,
		      double const *lower, double const *upper) const;
    void support(double *lower, double *upper, unsigned int length,
		 std::vector<double const *> const &parameters,
		 std::vector<std::vector<unsigned int> > const &dims) const;
    bool isSupportFixed(std::vector<bool> const &fixmask) const;
    unsigned int df(std::vector<std::vector<unsigned int> > const &dims) const;

};

#endif /* DCUT_H_ */
