#ifndef VARIANCE_MONITOR_FACTORY_H_
#define VARIANCE_MONITOR_FACTORY_H_

#include <model/MonitorFactory.h>

namespace jags {
namespace base {

    class VarianceMonitorFactory : public MonitorFactory
    {
      public:
	Monitor *getMonitor(std::string const &name, Range const &range, 
			    BUGSModel *model, std::string const &type,
			    std::string &msg);
	std::string name() const;
    };
    
}}

#endif /* VARIANCE_MONITOR_FACTORY_H_ */
