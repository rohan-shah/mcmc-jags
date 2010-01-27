#ifndef FUNC_COSH_H_
#define FUNC_COSH_H_

#include <function/ScalarFunction.h>

namespace bugs {

    /**
     * @short Hyperbolic cosine function
     * @see sinh
     * <pre>
     * y <- cosh(x)
     * </pre>
     */
    class Cosh : public ScalarFunction
    {
    public:
	Cosh ();
	double evaluate(std::vector<double const *> const &args) const;
    };

}

#endif /* FUNC_COSH_H_ */
