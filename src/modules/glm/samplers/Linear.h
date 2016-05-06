#ifndef LINEAR_H_
#define LINEAR_H_

#include "GLMBlock.h"

namespace jags {
namespace glm {

    /**
     * Conjugate sampler for normal linear models.
     */
    class Linear : public GLMBlock {
    public:
	/**
	 * Constructor. See GLMMethod#GLMMethod for an explanation of the
	 * parameters
	 */
	Linear(GraphView const *view,
	       std::vector<SingletonGraphView const *> const &sub_views,
	       std::vector<Outcome *> const &outcomes,
	       unsigned int chain);
    };
    
}}

#endif /* LINEAR_H_ */
