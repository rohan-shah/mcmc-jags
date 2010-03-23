#ifndef DSUM_H_
#define DSUM_H_

#include <distribution/ScalarDist.h>

/**
 * @short Sum of two discrete random variables
 */
class DSum : public ScalarDist {
public:
    DSum();

    double logLikelihood(double x, 
			 std::vector<double const *> const &parameters,
			 double const *lower, double const *upper) const;
    double randomSample(std::vector<double const *> const &parameters,
			double const *lower, double const *upper,
			RNG *rng) const;
    bool checkParameterDiscrete (std::vector<bool> const &mask) const;
    double l(std::vector<double const *> const &parameters) const;
    double u(std::vector<double const *> const &parameters) const;
    double typicalValue(std::vector<double const *> const &parameters,
			 double const *lower, double const *upper) const;
    bool isSupportFixed(std::vector<bool> const &fixmask) const;
    bool isDiscreteValued() const;
    unsigned int df() const;
    bool checkParameterValue(std::vector<double const *> const &params) const;
};

#endif /* DSUM_H_ */
