#ifndef DEVIANCE_MONITOR_H_
#define DEVIANCE_MONITOR_H_

#include <model/Monitor.h>

class StochasticNode;

namespace dic {

class DevianceMonitor : public Monitor {
    std::vector<std::vector<double> > _values; // sampled values
    StochasticNode const *_snode;
public:
    DevianceMonitor(Node const *node, unsigned int start, unsigned int thin); 
    unsigned int nchain() const;
    std::vector<unsigned int> dim() const;
    std::vector<double> const &value(unsigned int chain) const;
    void doUpdate();
    virtual void reserve(unsigned int niter);
};

}

#endif /* DEVIANCE_MONITOR_H_ */
