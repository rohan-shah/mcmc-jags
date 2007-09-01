#ifndef MONITOR_FACTORY_H_
#define MONITOR_FACTORY_H_

#include <string>

class Monitor;
class Node;

class MonitorFactory {
public:
    virtual ~MonitorFactory();
    /**
     * Creates a monitor of the given type. If the monitor cannot
     * be constructed, a null pointer is returned
     */
    virtual Monitor *
	getMonitor(Node const *node, unsigned int start,
		   unsigned int thin, std::string const &type) = 0;
};

#endif /* MONITOR_FACTORY_H_ */
