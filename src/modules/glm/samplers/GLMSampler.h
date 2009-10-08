#ifndef GLM_SAMPLER_H_
#define GLM_SAMPLER_H_

#include <sampler/ParallelSampler.h>

#include <vector>

class Updater;

namespace glm {

    class GLMSampler : public ParallelSampler
    {
	std::vector<Updater*> _sub_updaters;
    public:
	GLMSampler(Updater *updater, std::vector<Updater*> const &sub_updaters,
		   std::vector<SampleMethod*> const &methods);
	~GLMSampler();
    }; 

}

#endif /* GLM_SAMPLER_H_ */
