#ifndef CONJUGATE_F_SAMPLER_H_
#define CONJUGATE_F_SAMPLER_H_

#include <sampler/Sampler.h>

namespace glm {

    class ConjugateFMethod;

    /**
     * @short Sampler for precision parameter of random effects
     *
     * The Conjugate F method requires two GraphView objects, instead of
     * the usual single GraphView, as it works on hierarchical models.
     *
     * @see ConjugateFMethod
     */
    class ConjugateFSampler : public Sampler
    {
	GraphView *_gv2;
	std::vector<ConjugateFMethod*> _methods;
    public:
	ConjugateFSampler(GraphView *gv1, GraphView *gv2, 
			  std::vector<ConjugateFMethod*> const &methods);
	~ConjugateFSampler();
	void update(std::vector<RNG*> const &rngs);
	/**
	 * Returns false. This is not an adaptive sampler
	 */
	bool isAdaptive() const;
	/**
	 * Does nothing. This is not an adaptive sampler
	 */
	void adaptOff();
	/**
	 * Returns true. This is not an adaptive sampler.
	 */
	bool checkAdaptation() const;
	/**
	 * Returns "conjugateF"
	 */
	std::string name() const;
    };

}

#endif /* CONJUGATE_F_SAMPLER_H_ */
