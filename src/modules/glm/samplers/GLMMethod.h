#ifndef GLM_METHOD_H_
#define GLM_METHOD_H_

extern "C" {
#include <cs.h>
}

#include <string>
#include <vector>

class Graph;
class RNG;
class StochasticNode;
class Updater;

namespace glm {

    /**
     * Abstract method for generalized linear models.
     */
    class GLMMethod {
    protected:
	cs *_X;
	css *_symbol;
	bool _fixed;
	unsigned int _length_max;
	unsigned _nz_prior;
	std::vector<StochasticNode const*> const *_children;
	void symbolic(Updater const *updater);
	/**
	 * Calculates the design matrix of the GLM
	 */
	void calDesign(cs *X, Updater const *updater, unsigned int chain) 
	    const;
    public:
	GLMMethod(Updater const *updater);
	virtual ~GLMMethod();
	void update(Updater const *updater, unsigned int chain, RNG *rng) const;
	virtual std::string name() const = 0;
	virtual double getMean(unsigned int i, unsigned int chain) const = 0;
	virtual double getPrecision(unsigned int i, unsigned int chain) 
	    const = 0;
	virtual double getValue(unsigned int i, unsigned int chain) const = 0;
    };

}
#endif /* GLM_METHOD_H_ */
