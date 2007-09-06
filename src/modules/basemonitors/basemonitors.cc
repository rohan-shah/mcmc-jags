#include <Module.h>

#include "TraceMonitorFactory.h"

using std::vector;

namespace basemonitors {

    class BaseMonitorsModule: public Module {
    public:
	BaseMonitorsModule();
	~BaseMonitorsModule();
    };

    BaseMonitorsModule::BaseMonitorsModule() {

	insert(new TraceMonitorFactory);
    }

    BaseMonitorsModule::~BaseMonitorsModule() {

	vector<MonitorFactory*> const &mvec = monitorFactories();
	for (unsigned int i = 0; i < mvec.size(); ++i) {
	    delete mvec[i];
	}
    }

}

basemonitors::BaseMonitorsModule _base_montitors_module;

