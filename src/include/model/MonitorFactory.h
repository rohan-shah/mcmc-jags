#ifndef MONITOR_FACTORY_H_
#define MONITOR_FACTORY_H_

#include <string>
#include <vector>

class Monitor;
class Node;
class BUGSModel;
class Model;
class Range;

/**
 * @short Factory for Monitor objects
 */
class MonitorFactory {
public:
    virtual ~MonitorFactory();
    /**
     * Creates a monitor of the given type by name and range. If a 
     * monitor cannot be created, then a null pointer is returned.
     */
    virtual Monitor *getMonitor(std::string const &name, Range const &range,
				BUGSModel *model, std::string const &type) = 0;
};

#endif /* MONITOR_FACTORY_H_ */
