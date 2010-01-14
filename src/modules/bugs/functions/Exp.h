#ifndef FUNC_EXP_H_
#define FUNC_EXP_H_

#include <function/LinkFunction.h>

namespace bugs {

    /**
     * @short Exponential function
     * @see Log
     * <pre>
     * y <- exp(x)
     * </pre>
     */
    class Exp: public LinkFunction
    {
    public:
	Exp ();
	double inverseLink(double eta) const;
	double link(double mu) const;
	double grad(double eta) const;
    };

}

#endif /* FUNC_EXP_H_ */
