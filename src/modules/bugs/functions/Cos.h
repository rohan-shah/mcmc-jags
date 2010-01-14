#ifndef FUNC_COS_H_
#define FUNC_COS_H_

#include <function/ScalarFunction.h>

namespace bugs {

    /**
     * @short Cosine function
     * @see Sin
     * <pre>
     * y <- cos(x)
     * </pre>
     */
    class Cos : public ScalarFunction
    {
    public:
	Cos ();
	double evaluate(std::vector<double const *> const &args) const;
    };

}

#endif /* FUNC_COS_H_ */
