#ifndef MONITOR_FACTORY_H_
#define MONITOR_FACTORY_H_

#include <string>

class Monitor;
class Node;
class Model;

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
    /**
     * Adds monitors for "default" nodes to the given model. What
     * counts as a default node depends on the context, but the 
     * aim of this function is to allow users to conveniently define
     * multiple monitors they would normally specify individually
     * for the given model.
     */
    virtual void
	addDefaultMonitors(Model *model, unsigned int thin, 
			   std::string const &type) = 0; 
//FIXME: Who owns the new monitors?
};

#endif /* MONITOR_FACTORY_H_ */
