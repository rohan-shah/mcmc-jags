#ifndef GLM_SAMPLER_H_
#define GLM_SAMPLER_H_

#include <sampler/ParallelSampler.h>

#include <vector>

class GraphView;

namespace glm {

    /**
     * @short Base class for GLM samplers.
     */
    class GLMSampler : public ParallelSampler
    {
	std::vector<GraphView*> _sub_views;
    public:
	GLMSampler(GraphView *view, std::vector<GraphView*> const &sub_views,
		   std::vector<SampleMethod*> const &methods);
	~GLMSampler();
    }; 

}

#endif /* GLM_SAMPLER_H_ */
