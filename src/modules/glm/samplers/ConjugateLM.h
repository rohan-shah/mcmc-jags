#ifndef CONJUGATE_LM_H_
#define CONJUGATE_LM_H_

#include "GLMMethod.h"

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class ConjugateLM : public GLMMethod {
    public:
	double 
	    getMean(std::vector<StochasticNode const *> const &children,
		    unsigned int i, unsigned int chain) const;
	double 
	    getPrecision(std::vector<StochasticNode const *> const &children, 
			 unsigned int i, unsigned int chain) const;
	double 
	    getValue(std::vector<StochasticNode const *> const &children,
		     unsigned int i, unsigned int chain) const;
	std::string name() const;
    };
    
}

#endif /* CONJUGATE_LM_H_ */
