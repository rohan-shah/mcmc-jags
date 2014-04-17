#ifndef DIRICHLET_CAT_FACTORY_H_
#define DIRICHLET_CAT_FACTORY_H_

#include <sampler/SamplerFactory.h>

namespace jags {
    namespace mix {

	/**
	 * @short Factory object for DirichletCat samplers
	 */
	class DirichletCatFactory : public SamplerFactory
	{
	  public:
	    Sampler * makeSampler(std::vector<StochasticNode*> const &snodes, 
				  Graph const &graph) const;
	    std::vector<Sampler*>  
		makeSamplers(std::list<StochasticNode*> const &nodes, 
			     Graph const &graph) const;
	    std::string name() const;
	};
    }
}

#endif /* DIRICHLET_CAT_FACTORY_H_ */
