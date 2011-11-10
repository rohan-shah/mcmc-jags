#ifndef AUX_MIX_H_
#define AUX_MIX_H_

class RNG;

namespace glm {
    
    /**
     * @short Finite normal mixture representation 
     *
     * Abstract class for representing the outcome variable in a
     * generalized linear model as a finite mixture of normals. It is
     * used by AMMethod.
     *
     * @see AMMethod
     */
    class AuxMix {
      public:
	/**
	 * Virtual destructor
	 */
	virtual ~AuxMix();
	/**
	 * Value of the current normal approximation.
	 */
	virtual double value() const = 0;
	/**
	 * Precision of the current normal approximation.
	 */
	virtual double precision() const = 0;
	/**
	 * Samples a new auxiliary variable from its posterior
	 * distribution and updates the normal approximation..
	 */
	virtual void update(RNG *rng) = 0;
    };

}

#endif /* AUX_MIX_H_ */


