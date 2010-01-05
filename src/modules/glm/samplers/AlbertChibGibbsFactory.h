#ifndef ALBERT_CHIB_GIBBS_FACTORY_H_
#define ALBERT_CHIB_GIBBS_FACTORY_H_

#include "BinaryFactory.h"

namespace glm {

    /**
     * @shortFactory object for conjugate linear model sampler
     */
    class AlbertChibGibbsFactory : public BinaryFactory
    {
    public:
	AlbertChibGibbsFactory();
	BinaryGLM *newBinary(GraphView const *view, 
			     std::vector<GraphView const *> const &sub_views,
			     unsigned int chain) const;
    };

}

#endif /* ALBERT_CHIB_GIBBS_FACTORY_H_ */
