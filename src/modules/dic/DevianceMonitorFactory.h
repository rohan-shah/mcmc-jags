#ifndef DEVIANCE_MONITOR_FACTORY_H_
#define DEVIANCE_MONITOR_FACTORY_H_

#include <model/MonitorFactory.h>

namespace dic {

    class DevianceMonitorFactory : public MonitorFactory
    {
      public:
	Monitor *getMonitor(std::string const &name, Range const &range,
			    BUGSModel *model, std::string const &type,
			    std::string &msg);
	std::string name() const;
    };
    
}

#endif /* DEVIANCE_MONITOR_FACTORY_H_ */
