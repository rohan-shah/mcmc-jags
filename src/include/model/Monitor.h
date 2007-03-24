#ifndef MONITOR_H_
#define MONITOR_H_

class Node;

/**
 * @short Analyze sampled values 
 *
 * This is an abstract class for objects that analyze and/or store sampled
 * values from a given node. 
 */
class Monitor {
    Node const *_node;
public:
    Monitor(Node const *node);
    virtual ~Monitor();
    virtual void update(unsigned int iteration, unsigned int chain) = 0;
    virtual void reserve(unsigned int niter) = 0;
    Node const *node() const;
};

#endif
