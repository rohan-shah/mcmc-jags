#ifndef DINTERVAL_H_
#define DINTERVAL_H_

#include <distribution/VectorDist.h>

/**
 * @short Interval censored distribution
 * <pre>
 * i ~ dinterval(t, cutpoints[])
 * f(i|t) = 1 if t < cutpoints[i] and t >= cutpoints[i-1]
 *        = 0 otherwise
 * </pre>
 */
class DInterval : public VectorDist {
public:
    DInterval();
  
    double logLikelihood(double const *x, unsigned int length,
			 std::vector<double const *> const &parameters,
			 std::vector<unsigned int> const &lengths,
			 double const *lower, double const *upper) const;

    void randomSample(double *x, unsigned int length,
		      std::vector<double const *> const &parameters,
		      std::vector<unsigned int> const &lengths,
		      double const *lower, double const *upper, RNG *rng) const;
    void typicalValue(double *x, unsigned int length,
		      std::vector<double const *> const &parameters,
		      std::vector<unsigned int> const &lengths,
		      double const *lower, double const *upper) const;
    /**
     * Checks that cutpoints are in ascending order
     */
    bool checkParameterValue(std::vector<double const *> const &parameters,
			     std::vector<unsigned int> const &lengths) const;
    bool checkParameterLength(std::vector<unsigned int> const &lengths) const;
    void support(double *lower, double *upper, unsigned int length,
		 std::vector<double const *> const &parameters,
		 std::vector<unsigned int> const &lengths) const;
    bool isSupportFixed(std::vector<bool> const &fixmask) const;
    unsigned int df(std::vector<unsigned int> const &lengths) const;
    bool isDiscreteValued() const;
    unsigned int length(std::vector<unsigned int> const &params) const;
};

#endif /* DINTERVAL_H_ */
