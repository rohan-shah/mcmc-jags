#ifndef ALBERT_CHIB_GIBBS_FACTORY_H_
#define ALBERT_CHIB_GIBBS_FACTORY_H_

#include "BinaryFactory.h"

namespace glm {

    /**
     * @short Factory object for the Albert-Chib sampler with Gibbs sampling
     *
     * @see AlbertChib
     */
    class AlbertChibGibbsFactory : public BinaryFactory
    {
    public:
	AlbertChibGibbsFactory();
	/**
	 * Returns a newly allocated object of class AlbertChib for
	 * sampling binary GLMs with probit or logistic link. The
	 * object is constructed with parameter "gibbs" set to true,
	 * so that updating of the regression parameters is done
	 * element-wise by Gibbs sampling.
	 */
	BinaryGLM *newBinary(GraphView const *view, 
			     std::vector<GraphView const *> const &sub_views,
			     unsigned int chain) const;
    };

}

#endif /* ALBERT_CHIB_GIBBS_FACTORY_H_ */
