#ifndef GLM_METHOD_H_
#define GLM_METHOD_H_

#include <sampler/SampleMethod.h>
#include <sampler/GraphView.h>

#include <string>
#include <vector>

extern "C" {
#include <cholmod.h>
}

class Graph;
class RNG;
class StochasticNode;

enum GLMFamily {GLM_NORMAL, GLM_BERNOULLI, GLM_BINOMIAL, GLM_POISSON, 
		GLM_UNKNOWN};

namespace glm {

    /**
     * Abstract method for generalized linear models.
     */
    class GLMMethod : public SampleMethod {
	std::vector<double const *> _lp;
    protected:
	GraphView const *_view;
	unsigned int _chain;
	std::vector<GraphView const *> _sub_views;
	cholmod_sparse *_x;
	cholmod_factor *_factor;
    private:
	std::vector<bool> _fixed;
	unsigned int _length_max;
	unsigned _nz_prior;
	bool _init;
	void symbolic();
	void calDesign() const;
    public:
	GLMMethod(GraphView const *view, 
		  std::vector<GraphView const *> const &sub_views,
		  unsigned int chain, bool link);
	virtual ~GLMMethod();
	void updateLM(RNG *rng, bool stochastic = true, bool chol=true);
	void updateLMGibbs(RNG *rng);
	bool isAdaptive() const;
	bool adaptOff();
	void calCoef(double *&, cholmod_sparse *&);
	virtual	double getMean(unsigned int i) const;
	virtual std::string name() const = 0;
	virtual double getPrecision(unsigned int i) const = 0;
	virtual double getValue(unsigned int i) const = 0;
	virtual void updateAuxiliary(cholmod_dense *b, cholmod_factor *N, RNG *rng);
	static GLMFamily getFamily(StochasticNode const *snode);
    };

}
#endif /* GLM_METHOD_H_ */
