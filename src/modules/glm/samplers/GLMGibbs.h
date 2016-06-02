#ifndef GLM_GIBBS_H_
#define GLM_GIBBS_H_

#include "GLMMethod.h"

#include <vector>

namespace jags {

    struct RNG;
    class GraphView;
    class SingletonGraphView;

namespace glm {

    class Outcome;

    class GLMGibbs : public GLMMethod {
      public:
	/**
	 * Constructor.
	 *
	 * @param view Pointer to a GraphView object for all sampled nodes.
	 *
	 * @param sub_views Vector of pointers to SingletonGraphView
	 * objects with length equal to the number of sampled
	 * nodes. Each sub-view corresponds to a single sampled node.
	 *
	 * @param outcomes Vector of pointers to Outcome objects with length
	 * equal to the number of stochastic children of the sampled nodes.
	 * The GLMMethod objects takes ownership of each Outcome in the vector
	 * and frees the memory in the destructor.
	 * 
	 * @param chain Number of the chain (starting from 0) to which
	 * the sampling method will be applied.
	 *
	 * @param link Boolean flag that is passed to the utility
	 * function checkLinear when checking to see if we have a
	 * valid GLM. If link is true then the last deterministic
	 * descendents in view (i.e. those with no deterministic
	 * descendants) may be link nodes.
	 */
	GLMGibbs(GraphView const *view, 
		 std::vector<SingletonGraphView const *> const &sub_views,
		 std::vector<Outcome *> const &outcomes,
		 unsigned int chain);
	/**
	 * Updates the regression parameters element-wise (i.e. with
	 * Gibbs sampling).  Although Gibbs sampling less efficient
	 * than the block-updating provided by GLMBlock, it is more
	 * widely applicable.  In particular, if the regression
	 * parameters have a truncated normal prior, the model is
	 * still amenable to Gibbs sampling.
	 */
	void update(RNG *rng);
    };

}}

#endif /* GLM_GIBBS_H_ */
