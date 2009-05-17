#ifndef GLM_SAMPLER_H_
#define GLM_SAMPLER_H_

#include <sampler/Sampler.h>

namespace glm {

    class GLMMethod;

    class GLMSampler : public Sampler
    {
	GLMMethod  *_method;
    public:
	GLMSampler(std::vector<StochasticNode*> const &nodes, 
		   GLMMethod *method, Graph const &graph);
	~GLMSampler();
	void update(std::vector<RNG*> const &);
	bool isAdaptive() const;
	bool adaptOff();
	std::string name() const;
    };

}

#endif /* GLM_SAMPLER_H_ */
