#include <config.h>
#include <string>

#include "GLMSampler.h"
#include "GLMMethod.h"

using std::string;
using std::vector;

namespace glm {

    GLMSampler::GLMSampler(Updater *updater, GLMMethod *method)
	: Sampler(updater), _updater(updater), _method(method)
    {
	//_method->initialize(this, graph);
    }

    GLMSampler::~GLMSampler()
    {
	delete _method;
    }

    void GLMSampler::update(vector<RNG*> const &rngs)
    {
	for (unsigned int ch = 0; ch < rngs.size(); ++ch) {
	    _method->update(_updater, ch, rngs[ch]);
	}
    }

    bool GLMSampler::isAdaptive() const
    {
	return false;
    }

    bool GLMSampler::adaptOff()
    {
	return true;
    }

    string GLMSampler::name() const
    {
	return _method->name();
    }

}
