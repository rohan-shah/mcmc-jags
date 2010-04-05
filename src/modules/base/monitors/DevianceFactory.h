#ifndef DEVIANCE_FACTORY_H_
#define DEVIANCE_FACTORY_H_

#include <model/MonitorFactory.h>

namespace base {

    class DevianceFactory : public MonitorFactory
    {
    public:
	Monitor *getMonitor(std::string const &name, Range const &range, 
			    BUGSModel *model, std::string const &type);
	std::string name() const;
    };

}

#endif /* DEVIANCE_FACTORY_H_ */
