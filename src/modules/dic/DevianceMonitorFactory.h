#ifndef DEVIANCE_MONITOR_FACTORY_H_
#define DEVIANCE_MONITOR_FACTORY_H_

#include <model/MonitorFactory.h>

namespace dic {

class DevianceMonitorFactory : public MonitorFactory
{
public:
    Monitor *getMonitor(Node const *node, unsigned int start,
			unsigned int thin, std::string const &type);
};

}

#endif /* DEVIANCE_MONITOR_FACTORY_H_ */
