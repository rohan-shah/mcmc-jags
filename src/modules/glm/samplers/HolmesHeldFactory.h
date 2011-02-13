#ifndef HOLMES_HELD_FACTORY_H_
#define HOLMES_HELD_FACTORY_H_

#include "BinaryFactory.h"
#include "BinaryGLM.h"

namespace glm {

    /**
     * @shortFactory object for conjugate linear model sampler
     */
    class HolmesHeldFactory : public BinaryFactory
    {
    public:
	HolmesHeldFactory();
	BinaryGLM *newBinary(GraphView const *view, 
			     std::vector<GraphView const *> const &sub_views,
			     unsigned int chain) const;
	/**
	 * Holmes-Held method requires a fixed design matrix
	 */
	bool fixedGLM() const;
    };

}

#endif /* HOLMES_HELD_FACTORY_H_ */
