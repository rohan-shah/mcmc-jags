#ifndef FUNC_PHI_H_
#define FUNC_PHI_H_

#include <function/InverseLinkFunc.h>

namespace bugs {

    /**
     * @short Standard normal distribution function
     * @see Probit
     * <pre>
     * p <- phi(x)
     * probit(p) <- x
     * p = Prob(X < x) where X ~ N(0,1)
     * </pre>
     */
    class Phi : public InverseLinkFunc
    {
    public:
	Phi ();
	double inverseLink(double eta) const;
	double link(double mu) const;
	double gradLink(double mu) const;
    };

}

#endif /* FUNC_PHI_H_ */
