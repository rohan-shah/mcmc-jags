#ifndef LINEAR_H_
#define LINEAR_H_

#include "GLMMethod.h"

namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class Linear : public GLMMethod {
	bool _gibbs;
    public:
	Linear(GraphView const *view,
	       std::vector<GraphView const *> const &sub_views,
	       unsigned int chain, bool gibbs);
	double getPrecision(unsigned int i) const;
	double getValue(unsigned int i) const;
	bool update(RNG *rng);
	std::string name() const;
    };
    
}

#endif /* LINEAR_H_ */
