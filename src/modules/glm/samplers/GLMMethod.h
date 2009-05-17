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

namespace glm {

    class GLMSampler;

    /**
     * Abstract method for generalized linear models.
     */
    class GLMMethod {
    protected:
	cs *_beta;
	css *_symbol;
	bool _fixed;
	unsigned int _length_max;
	unsigned _nz_prior;
	std::vector<StochasticNode const*> const *_children;
	void symbolic(GLMSampler const *sampler);
	void calBeta(cs *_beta, GLMSampler *sampler, unsigned int chain) const;
    public:
	GLMMethod();
	virtual ~GLMMethod();
	void initialize(GLMSampler *sampler, Graph const &graph);
	void update(GLMSampler *sampler, unsigned int chain, RNG *rng) const;
	virtual std::string name() const = 0;
	virtual double 
	    getMean(std::vector<StochasticNode const *> const &children,
		    unsigned int i, unsigned int chain) const = 0;
	virtual double 
	    getPrecision(std::vector<StochasticNode const *> const &children, 
			 unsigned int i, unsigned int chain) const = 0;
	virtual double 
	    getValue(std::vector<StochasticNode const *> const &children,
		     unsigned int i, unsigned int chain) const = 0;
    };

}
#endif /* GLM_METHOD_H_ */
