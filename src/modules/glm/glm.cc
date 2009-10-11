#include <Module.h>
#include <distribution/DistTab.h>

#include "samplers/ConjugateLMFactory.h"
#include "samplers/ProbitFactory.h"

using std::vector;

namespace glm {
    
    class GLMModule : public Module {
    public:
	GLMModule();
	~GLMModule();
    };
    
    GLMModule::GLMModule() 
	: Module("glm")
    {
	
	insert(new ConjugateLMFactory);
	insert(new ProbitFactory);
	
    }
    
    GLMModule::~GLMModule() {
	
	vector<SamplerFactory*> const &svec = samplerFactories();
	for (unsigned int i = 0; i < svec.size(); ++i) {
	    delete svec[i];
	}
    }
}

glm::GLMModule _glm_module;
