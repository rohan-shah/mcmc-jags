#ifndef POPT_MONITOR_FACTORY_H_
#define POPT_MONITOR_FACTORY_H_

#include <model/MonitorFactory.h>

namespace dic {

class PoptMonitorFactory : public MonitorFactory
{
public:
    Monitor *getMonitor(std::string const &name, Range const &range,
			BUGSModel *model, std::string const &type);
};

}

#endif /* POPT_MONITOR_FACTORY_H_ */
