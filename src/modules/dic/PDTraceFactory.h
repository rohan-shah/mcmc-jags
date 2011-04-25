#ifndef PD_TRACE_FACTORY_H_
#define PD_TRACE_FACTORY_H_

#include <model/MonitorFactory.h>

namespace dic {

    class PDTraceFactory : public MonitorFactory
    {
      public:
	Monitor *getMonitor(std::string const &name, Range const &range,
			    BUGSModel *model, std::string const &type,
			    std::string &msg);
	std::string name() const;
    };
    
}

#endif /* PD_TRACE_FACTORY_H_ */
