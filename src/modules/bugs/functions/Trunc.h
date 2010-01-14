#ifndef FUNC_TRUNC_H_
#define FUNC_TRUNC_H_

#include <function/ScalarFunction.h>

namespace bugs {

/**
 * @short Round to integer, towards zero
 * <pre>
 * y <- trunc(x)
 * </pre>
 */
    class Trunc : public ScalarFunction
    {
    public:
	Trunc ();
	double evaluate(std::vector<double const *> const &args) const;
	bool isDiscreteValued(std::vector<bool> const &mask) const;
    };

}

#endif /* FUNC_TRUNC_H_ */
