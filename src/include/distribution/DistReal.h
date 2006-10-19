#ifndef DIST_REAL_H_
#define DIST_REAL_H_

#include <distribution/DistScalarRmath.h>

/**
 * Base class for real-valued distributions.
 *
 * @short Real-valued distributions
 */
class DistReal : public DistScalarRmath
{
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
     */
    DistReal(std::string const &name, unsigned int npar,
	     Support support, bool boundable);
    double logLikelihood(double x,
			 std::vector<double const *> const &parameters) const;
    double randomSample(std::vector<double const *> const &parameters,
			RNG *rng) const;
};

#endif /* DIST_REAL_H_ */
