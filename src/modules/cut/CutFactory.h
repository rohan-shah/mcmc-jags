#ifndef CUT_FACTORY_H_
#define CUT_FACTORY_H_

#include <sampler/SamplerFactory.h>

namespace cut {
    
/**
 * @short Factory for cut samplers 
 */
    class CutFactory : public SamplerFactory
    {
    public:
	virtual ~CutFactory();
	Sampler * makeSampler(std::set<StochasticNode*> const &nodes, 
			      Graph const &graph) const;
    };
    
}

#endif /* CUT_FACTORY_H_ */
