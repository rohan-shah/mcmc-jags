#include <Module.h>
#include <RngStreamFactory.h>

using std::vector;

namespace lecuyer {

    class LecuyerModule : public Module {

    public:
	LecuyerModule();
	~LecuyerModule();
    };

    LecuyerModule::LecuyerModule() 
	: Module("lecuyer") 
    {
	
	insert(new RngStreamFactory);
	
    }
    
    LecuyerModule::~LecuyerModule() {
	
	vector<RNGFactory*> const &rvec = rngFactories();
	for (unsigned int i = 0; i < rvec.size(); ++i) {
	    delete rvec[i];
	}
    }
    
}

lecuyer::LecuyerModule _lecuyer_module;
