#ifndef SUM_METHOD_H_
#define SUM_METHOD_H_

#include <sampler/Slicer.h>

#include <vector>

namespace jags {

    class StochasticNode;
    class DeterministicNode;
    class GraphView;
    
    namespace bugs {

	/**
	 * @short Sampler for stochastic parents of sum nodes
	 */
	class SumMethod : public Slicer
	{
	    GraphView const *_gv;
	    unsigned int _chain;
	    double _sum;
	    bool _discrete;
	    std::vector<double> _x;
	    unsigned int _i, _j;
	  public:
	    SumMethod(GraphView const *gv, unsigned int chain);
	    ~SumMethod();
	    void update(RNG *);
	    void setValue(double x);
	    double value() const;
	    void getLimits(double *lower, double *upper) const;
	    std::string name() const;
	    double logDensity() const;
	    static StochasticNode const *
		isCandidate(StochasticNode *snode, Graph const &graph);
	    static bool canSample(std::vector<StochasticNode *> const &nodes, 
				  Graph const &graph);
	};

    } //namespace bugs
} //namespace jags

#endif /* SUM_METHOD_H_ */
