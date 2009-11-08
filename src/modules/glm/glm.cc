#include <Module.h>

#include "samplers/LinearFactory.h"
#include "samplers/LinearGibbsFactory.h"
//#include "samplers/ProbitFactory.h"
//#include "samplers/LogitFactory.h"
#include "samplers/IWLSFactory.h"
#include "samplers/HolmesHeldFactory.h"

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
	//insert(new IWLSFactory);
	insert(new LinearFactory);
	insert(new LinearGibbsFactory);
	//insert(new ProbitFactory);
        //insert(new LogitFactory);
	insert(new HolmesHeldFactory);
    }
    
    GLMModule::~GLMModule() {
	
	vector<SamplerFactory*> const &svec = samplerFactories();
	for (unsigned int i = 0; i < svec.size(); ++i) {
	    delete svec[i];
	}
    }
}

glm::GLMModule _glm_module;
