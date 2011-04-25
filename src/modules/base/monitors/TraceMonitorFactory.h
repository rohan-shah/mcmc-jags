#ifndef TRACE_MONITOR_FACTORY_H_
#define TRACE_MONITOR_FACTORY_H_

#include <model/MonitorFactory.h>

namespace base {

    class TraceMonitorFactory : public MonitorFactory
    {
      public:
	Monitor *getMonitor(std::string const &name, Range const &range, 
			    BUGSModel *model, std::string const &type,
			    std::string &msg);
	std::string name() const;
    };
    
}

#endif /* TRACE_MONITOR_FACTORY_H_ */
