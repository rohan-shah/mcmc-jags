#ifndef FUNC_ARCSINH_H_
#define FUNC_ARCSINH_H_

#include <function/ScalarFunction.h>

namespace bugs {

    /**
     * @short Inverse hyperbolic sine function
     * @see sinh
     * <pre>
     * y <- arcsinh(x)
     * </pre>
     */
    class ArcSinh : public ScalarFunction
    {
    public:
	ArcSinh ();
	std::string alias() const;
	double evaluate(std::vector<double const *> const &args) const;
    };

}

#endif /* FUNC_ARCSINH_H_ */
