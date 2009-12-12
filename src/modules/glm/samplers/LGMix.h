#ifndef LG_MIX_H_
#define LG_MIX_H_

#include <cstdlib>
#include <cmath>
#include <algorithm>

class RNG;

using std::copy;

namespace glm {

    /**
     * Represents a negative log gamma distribution with shape n and
     * scale 1 as a finite mixture of normals
     */
    class LGMix {
	double _nlast;
	int _r;
	int _ncomp;	
	double _weights[10];
	double _means[10];
	double _variances[10];
	void updateNExact(double n);
	void updateNApprox(double n);
	void updateN(double n);
    public:
	LGMix(double n);
	/**
	 * Updates the mixture representation
	 * @param z Value sampled from log gamma distribution
	 * @param n Value of n, which may be different from value used in
	 *          constructor
	 * @param rng Random number generator
	 */
	void update(double z, double n, RNG *rng);
	double mean() const;
	double variance() const;
	double precision() const;
    };

}

#endif /* LG_MIX_H */
