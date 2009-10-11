#include <Module.h>
#include <distribution/DistTab.h>

#include <distributions/DMState.h>

#include <functions/Mexp.h>

using std::vector;

namespace msm {

    class MSMModule : public Module {
    public:
	MSMModule();
	~MSMModule();
    };
    
    MSMModule::MSMModule() 
	: Module("msm")
    {
	
	insert(new DMState);
	insert(new Mexp);
    }
    
    MSMModule::~MSMModule() {
	
	vector<Distribution*> const &dvec = distributions();
	for (unsigned int i = 0; i < dvec.size(); ++i) {
	    delete dvec[i];
	}
	
	vector<Function*> const &fvec = functions();
	for (unsigned int i = 0; i < fvec.size(); ++i) {
	    delete fvec[i];
	}
    }
}
 
msm::MSMModule _msm_module;
    
