#ifndef GLM_METHOD_H_
#define GLM_METHOD_H_

extern "C" {
#include <cs.h>
}

#include <sampler/SampleMethod.h>
#include <sampler/Updater.h>

#include <string>
#include <vector>

class Graph;
class RNG;
class StochasticNode;

namespace glm {

    /**
     * Abstract method for generalized linear models.
     */
    class GLMMethod : public SampleMethod {
    protected:
	Updater const *_updater;
	unsigned int _chain;
	std::vector<Updater const *> _sub_updaters;
	cs *_X;
	css *_symbol;
    private:
	bool _fixed;
	unsigned int _length_max;
	unsigned _nz_prior;
	bool _init;
	void symbolic();
	void calDesign(cs *X) const;
    public:
	GLMMethod(Updater const *updater, 
		  std::vector<Updater const *> const &sub_updaters,
		  unsigned int chain, bool link);
	virtual ~GLMMethod();
	void update(RNG *rng);
	bool isAdaptive() const;
	bool adaptOff();
	virtual std::string name() const = 0;
	virtual double getMean(unsigned int i) const = 0;
	virtual double getPrecision(unsigned int i) const = 0;
	virtual double getValue(unsigned int i) const = 0;
	virtual void initAuxiliary(RNG *rng) = 0;
	virtual void updateAuxiliary(double *b, csn const *N, RNG *rng) = 0;
    };

}
#endif /* GLM_METHOD_H_ */
