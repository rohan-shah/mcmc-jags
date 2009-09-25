#ifndef GLM_SAMPLER_H_
#define GLM_SAMPLER_H_

#include <sampler/Sampler.h>

class Updater;
class GLMMethod;

namespace glm {

    class GLMMethod;

    class GLMSampler : public Sampler
    {
	Updater const *_updater;
	GLMMethod  *_method;
    public:
	GLMSampler(Updater *updater, GLMMethod *method);
	~GLMSampler();
	void update(std::vector<RNG*> const &);
	bool isAdaptive() const;
	bool adaptOff();
	std::string name() const;
    };

}

#endif /* GLM_SAMPLER_H_ */
