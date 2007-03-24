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
    std::vector<unsigned int> _niter;
 public:
  /**
   * Constructor for a TraceMonitor of finite size
   * @param node Node to sample
   * @param start Iteration at which to start monitoring
   * @param end Iteration at which to end monitoring
   * @param thin Thinning interval for monitor.  The monitor only
   * stores iteration for which (iteration number)%thin == 0.
   */
   //TraceMonitor(Node const *node, unsigned int start, unsigned int end,
   //unsigned int thin); FIXME
  /**
   * Constructor for an open-ended TraceMonitor. The Monitor will
   * continue to sample values until it is deleted
   * @param node Node to sample
   * @param start Iteration at which to start monitoring
   * @param thin Thinning interval for monitor.  The monitor only
   * stores iterations for which (iteration - start) % thin == 0.
   */
  TraceMonitor(Node const *node, unsigned int start, unsigned int thin);
  ~TraceMonitor();
  /**
   * Iteration number at which the node started monitoring.  
   */
  unsigned int start() const; 
  /**
   * Last monitored iteration
   */
  unsigned int end(unsigned int chain) const;
  /**
   * Thinning interval of monitor
   */
  unsigned int thin() const;
  /**
   * The number of monitored iterations
   */
  unsigned int niter(unsigned int chain) const;                       
  /**
   * The number of monitored iterations between two limits
   */
  //unsigned int size(unsigned int start, unsigned int end) const;
  /**
   * Updates the monitor
   * @param iteration The current iteration number.
   */
  void update(unsigned int iteration, unsigned int chain);
  /**
   * Returns a reference to the vector of monitored values for the
   * given chain
   */
  std::vector<double> const &values(unsigned int chain) const; 
  void reserve(unsigned int niter);
};

/** Returns true if the monitor has the same size for all chains */
bool isSynchronized(TraceMonitor const *monitor);

#endif
