#ifndef DINTERVAL_H_
#define DINTERVAL_H_

#include <distribution/VectorDist.h>

namespace bugs {

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
  
    double logDensity(double const *x, unsigned int length, PDFType type,
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
    bool isDiscreteValued(std::vector<bool> const &mask) const;
    unsigned int length(std::vector<unsigned int> const &params) const;
    double KL(std::vector<double const *> const &par1,
	      std::vector<double const *> const &par2,
	      std::vector<unsigned int> const &lengths) const;
};

}

#endif /* DINTERVAL_H_ */
