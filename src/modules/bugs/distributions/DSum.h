#ifndef DSUM_H_
#define DSUM_H_

#include <distribution/DistScalar.h>

/**
 * @short Sum of two discrete random variables
 */
class DSum : public DistScalar {
public:
    DSum();

    double logLikelihood(double x, 
			 std::vector<double const *> const &parameters) const;
    double randomSample(std::vector<double const *> const &parameters,
			RNG *rng) const;
    bool checkParameterDiscrete (std::vector<bool> const &mask) const;
    bool checkParameterValue(std::vector<double const *> const &parameters,
			     std::vector<std::vector<unsigned int> > const &dims)
	const;
    double l(std::vector<double const *> const &parameters) const;
    double u(std::vector<double const *> const &parameters) const;
    double typicalValue(std::vector<double const *> const &parameters) const;
    bool isSupportFixed(std::vector<bool> const &fixmask) const;
    unsigned int df(std::vector<std::vector<unsigned int> > const &dims) const;
};

#endif /* DSUM_H_ */
