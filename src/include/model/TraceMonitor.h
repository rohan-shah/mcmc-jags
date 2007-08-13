#ifndef TRACE_MONITOR_H_
#define TRACE_MONITOR_H_

#include <model/Monitor.h>
#include <vector>

class Node;

/**
 * @short Stores sampled values of a given Node
 */
class TraceMonitor : public Monitor {
    Node const *_node;                  
    const unsigned int _start;                  
    const unsigned int _thin;      // thinning interval (=1/frequency)
    std::vector<std::vector<double> > _values; // sampled values
    unsigned int _niter;
 public:
  /**
   * Constructs a TraceMonitor. 
   * @param node Node to sample
   * @param start Iteration at which to start monitoring
   * @param thin Thinning interval for monitor.  The monitor only
   * stores iterations for which (iteration - start) % thin == 0.
   */
  TraceMonitor(Node const *node, unsigned int start, unsigned int thin);
  ~TraceMonitor();
  /**
   * @returns the iteration number at which the node started monitoring.  
   */
  unsigned int start() const; 
  /**
   * @returns The last monitored iteration
   */
  unsigned int end() const;
  /**
   * @returns the thinning interval of the monitor
   */
  unsigned int thin() const;
  /**
   * The number of monitored iterations
   */
  unsigned int niter() const;                       
  /**
   * Updates the monitor. If the iteration number coincides with
   * the thinning interval, then the current value of the node
   * is pushed onto the back of the value vector for the monitor.
   *
   * @param iteration The current iteration number.
   */
  void update(unsigned int iteration);
  /**
   * @returns the vector of monitored values for the given chain
   */
  std::vector<double> const &values(unsigned int chain) const; 
  /**
   * Reserves memory for future updates. Sufficient memory is 
   * reserved for storage of future samples to avoid re-allocation
   * of memory for the next niter iterations.
   *
   * @param niter number of future iterations to reserve. 
   */
  void reserve(unsigned int niter);
};

#endif
