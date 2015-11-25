#ifndef SUM_METHOD_H_
#define SUM_METHOD_H_

#include <sampler/MutableSampleMethod.h>

#include <vector>

namespace jags {

    class StochasticNode;
    class DeterministicNode;
    class GraphView;
    
    namespace bugs {

	/**
	 * @short Sampler for stochastic parents of sum nodes
	 */
	class SumMethod : public MutableSampleMethod
	{
	    GraphView const *_gv;
	    unsigned int _chain;
	    double _sum;
	    bool _discrete;
	    std::vector<double> _x;
	    unsigned int _i, _j;
	    StochasticNode *_sumchild;
	    bool _fast;
	    double _sumdiff;
	    unsigned int _iter;
	    double _width;
	    int _max;
	    bool _adapt;
	  public:
	    SumMethod(GraphView const *gv, unsigned int chain);
	    ~SumMethod();
	    void update(RNG *);
	    void setValue(double x);
	    double value() const;
	    void getLimits(double *lower, double *upper) const;
	    std::string name() const;
	    double logDensity() const;
	    void updateStep(RNG *rng);
	    bool isAdaptive() const;
	    void adaptOff();
	    bool checkAdaptation() const;
	    static StochasticNode *
		isCandidate(StochasticNode *snode, Graph const &graph);
	    static bool canSample(std::vector<StochasticNode *> const &nodes, 
				  Graph const &graph);
	};

    } //namespace bugs
} //namespace jags

#endif /* SUM_METHOD_H_ */
