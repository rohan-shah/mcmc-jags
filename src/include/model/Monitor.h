#ifndef MONITOR_H_
#define MONITOR_H_

#include <vector>

class Node;

/**
 * @short Analyze sampled values 
 *
 * This is an abstract class for objects that analyze and/or store sampled
 * values from a given node. 
 */
class Monitor {
    std::vector<Node const *> _nodes;
public:
    Monitor(std::vector<Node const *> const &nodes);
    virtual ~Monitor();
    virtual void update(unsigned int iteration) = 0;
    virtual void reserve(unsigned int niter) = 0;
    std::vector<Node const *> const &nodes() const;
};

#endif
