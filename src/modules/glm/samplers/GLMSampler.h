#ifndef GLM_SAMPLER_H_
#define GLM_SAMPLER_H_

#include <sampler/MutableSampler.h>

#include <vector>

namespace jags {

    class GraphView;
    class SingletonGraphView;

namespace glm {

    /**
     * @short Base class for GLM samplers.
     */
    class GLMSampler : public MutableSampler
    {
	std::vector<SingletonGraphView*> _sub_views;
    public:
	/**
	 * Constructor.
	 *
	 * @param view Pointer to a GraphView object for all sampled nodes.
	 *
	 * @param sub_views Vector of pointers to SingletonGraphView
	 * objects with length equal to the number of sampled
	 * nodes. Each sub-view corresponds to a single sampled node.
	 * The GLMSampler object takes ownership of these sub-views
	 * and deletes them when its destructor is called.
	 *
	 * @param methods Vector of sampling methods
	 */
	GLMSampler(GraphView *view, 
		   std::vector<SingletonGraphView*> const &sub_views,
		   std::vector<MutableSampleMethod*> const &methods,
		   std::string const &name);
	/**
	 * Destructor
	 * 
	 * Deletes the sub-views passed to the constructor.
	 */
	~GLMSampler();
    }; 

}}

#endif /* GLM_SAMPLER_H_ */
