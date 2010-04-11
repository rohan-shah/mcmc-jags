#ifndef DIRICHLET_INFO_H_
#define DIRICHLET_INFO_H_

class StochasticNode;

namespace mix {

    /**
     * Helper class to store information about nodes with a Dirichlet
     * distribution. This is used to invoke special rules for the
     * NormMix sample method.
     */
    struct DirichletInfo 
    {
	unsigned int start;
	unsigned int end;
	unsigned int length;
	double sum;
	double shape;
	DirichletInfo(StochasticNode const *snode, unsigned int start,
		      unsigned int chain);
	double gammaPenalty() const;
    };

}

#endif /* DIRICHLET_INFO_H_ */
