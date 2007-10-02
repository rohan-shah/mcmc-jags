#ifndef FUNC_SIN_H_
#define FUNC_SIN_H_

#include <function/ScalarFunc.h>

namespace bugs {

    /**
     * @short Sine function
     * @see Cos
     * <pre>
     * y <- sin(x)
     * </pre>
     */
    class Sin : public ScalarFunc
    {
    public:
	Sin ();
	double evaluateScalar(std::vector<double const *> const &args) const;
    };
}

#endif /* FUNC_SIN_H_ */
