#ifndef ALBERT_CHIB_FACTORY_H_
#define ALBERT_CHIB_FACTORY_H_

#include "BinaryFactory.h"

namespace glm {

    /**
     * @short Factory object for the Albert-Chib sampler with block updating
     *
     * @see AlbertChib
     */
    class AlbertChibFactory : public BinaryFactory
    {
    public:
	AlbertChibFactory();
	/**
	 * Returns a newly allocated object of class AlbertChib for
	 * sampling binary GLMs with probit or logistic link.
	 */
	BinaryGLM *newBinary(GraphView const *view, 
			     std::vector<GraphView const *> const &sub_views,
			     unsigned int chain) const;
    };

}

#endif /* ALBERT_CHIB_FACTORY_H_ */
