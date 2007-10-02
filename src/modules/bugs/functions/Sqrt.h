#ifndef FUNC_SQRT_H_
#define FUNC_SQRT_H_

#include <function/ScalarFunc.h>

namespace bugs {

    /**
     * @short Square root function
     * <pre>
     * sigma <- sqrt(tau)
     * tau = sigma * sigma; tau > 0
     * </pre>
     */
    class Sqrt : public ScalarFunc
    {
    public:
	Sqrt ();
	double evaluateScalar(std::vector<double const *> const &args) const;
	bool checkScalarValue(std::vector<double const *> const &args) const;
    };
}

#endif /* FUNC_SQRT_H_ */
