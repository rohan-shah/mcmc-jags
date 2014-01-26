#ifndef CAT_DIRICHLET_H_
#define CAT_DIRICHLET_H_

#include <sampler/SampleMethodNoAdapt.h>

namespace jags {

    class GraphView;

    namespace mix {

	class DirichletCat;

	/**
	 * @short Collapsed sampler for categorical nodes in a
	 * Dirichlet mixture model.
	 */
	class CatDirichlet : public SampleMethodNoAdapt {
	    GraphView const *_gv;
	    DirichletCat *_dc;
	    unsigned int _chain;
	    unsigned int _size;
	  public:
	    CatDirichlet(GraphView const *gv, DirichletCat *dc, 
			 unsigned int chain);
	    void update(RNG *rng);
	};
    } 
}

#endif /* CAT_DIRICHLET_H_ */
