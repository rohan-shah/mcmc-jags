#ifndef LG_MIX_H_
#define LG_MIX_H_

#include <cstdlib>
#include <cmath>
#include <algorithm>

class RNG;

using std::copy;

namespace glm {

    /**
     * @short Finite mixture representation of negative log-gamma distribution
     *
     * Represents a negative log gamma distribution with integer shape n > 0 and
     * scale 1 as a finite mixture of normals. This is used by the
     * AMMethod sampling method for Poisson and binomial GLMs.
     */
    class LGMix {
	double _nlast;
	int _r;
	int _ncomp;	
	double _weights[10];
	double _means[10];
	double _variances[10];
	void updateNExact(int n);
	void updateNApprox(double n);
	void updateN(double n);
    public:
	LGMix(double n);
	/**
	 * Updates the mixture representation
	 *
	 * @param z Value sampled from negative log gamma distribution
	 * @param n Value of n, which may be different from value used in
	 *          constructor. 
	 * @param rng Random number generator
	 */
	void update(double z, double n, RNG *rng);
	/**
	 * Rturns the mean of the current normal component
	 */
	double mean() const;
	/**
	 * Returns the variance of the current normal component
	 */
	double variance() const;
	/**
	 * Returns the precision of the current normal component
	 */
	double precision() const;
    };

}

#endif /* LG_MIX_H */
