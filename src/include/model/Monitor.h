#ifndef MONITOR_H_
#define MONITOR_H_

#include <sarray/SArray.h>

#include <vector>
#include <string>

class Node;

/**
 * @short Analyze sampled values 
 *
 * This is an abstract class for objects that analyze and/or store sampled
 * values from a given node. 
 */
class Monitor {
    std::string _type;
    std::vector<Node const *> _nodes;
    std::string _name;
    std::vector<std::string> _elt_names;
public:
    Monitor(std::string const &type, std::vector<Node const *> const &nodes);
    Monitor(std::string const &type, Node const *node);
    virtual ~Monitor();
    /**
     * Updates the monitor. If the iteration number coincides with
     * the thinning interval, then the doUpdate function is called.
     */
    virtual void update() = 0;
    /**
     * Returns the vector of nodes from which the monitors value is
     * derived.
     */
    std::vector<Node const *> const &nodes() const;
    /**
     * The type of monitor. Each subclass must define have a unique
     * type, which is common to all Monitors of that class. The type
     * is used by the user-interface to identify the subclass of Monitor.
     */
    std::string const &type() const;
    /**
     * Returns true if the monitor has a single value for multiple chains
     */
    virtual bool poolChains() const = 0;
    /**
     * Returns true if the monitor has a single value for multiple iterations
     */
    virtual bool poolIterations() const = 0;
    /**
     * Returns the dimension of a single monitored value, which may
     * be replicated over chains and over iterations
     */
    virtual std::vector<unsigned int> dim() const = 0;
    /**
     * The vector of monitored values for the given chain
     */
    virtual std::vector<double> const &value(unsigned int chain) const = 0;
    /**
     * Reserves memory for future updates. Sufficient memory is
     * reserved for storage of future samples to avoid re-allocation
     * of memory for the next niter iterations.
     *
     * @param niter number of future iterations to reserve. 
     */
    virtual void reserve(unsigned int niter) = 0;
     /**
      * Dumps the monitored values to an SArray. 
      *
      * The SArray will have informative dimnames. In particular, the
      * dimnames "iteration" and "chain" are used if there are
      * distinct values for each iteration and each chain,
      * respectively.
      *
      * @param flat Indicates whether value should be flattened, so
      * that the value for a single iteration and single chain is a
      * vector.
      */
     SArray dump(bool flat = false) const;
     /**
      * Returns the name of the monitor
      */
     std::string const &name() const;
     /**
      * Sets the name of the monitor
      */
     void setName(std::string const &name);
     /**
      * Returns the names of individual elements
      */
     std::vector<std::string> const &elementNames() const;
     /**
      * Sets the element names. The length of the string must be
      * conform to the dimensions of the monitor, as returned by the
      * dim1 member function.
      */
     void setElementNames(std::vector<std::string> const &names);
};

#endif
