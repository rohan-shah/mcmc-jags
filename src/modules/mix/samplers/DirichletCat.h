#ifndef DIRICHLET_CAT_H_
#define DIRICHLET_CAT_H_

#include <sampler/SampleMethodNoAdapt.h>

#include <map>
#include <vector>

namespace jags {

    class MixtureNode;
    class Node;
    class GraphView;

    namespace mix {
	/**
	 * @short Block sampler for Dirichlet nodes in a mixture model.  
	 *
	 */
	class DirichletCat : public SampleMethodNoAdapt {
	    GraphView const *_gv;
	    std::map<Node const*, std::vector<double> > _parmap;
	    std::vector<MixtureNode const *> _mixparents;
	    unsigned int _chain;
	    unsigned int _size;
	    void updateParMap();
	  public:
	    DirichletCat(GraphView const *gv, unsigned int chain);
	    void update(RNG *rng);
	    static bool canSample(GraphView const *gv);
	    /**
	     * Returns the active parameter for the stochastic child
	     * with index i. This is used by the companion sampler
	     * CatDirichlet in the case where the stochastic children
	     * are also unobserved.
	     */
	    std::vector<double> &getActiveParameter(unsigned int i);
	};
    } 
}

#endif /* DIRICHLET_CAT_H_ */
