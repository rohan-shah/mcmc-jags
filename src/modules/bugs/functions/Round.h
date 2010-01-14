#ifndef FUNC_ROUND_H_
#define FUNC_ROUND_H_

#include <function/ScalarFunction.h>

namespace bugs {

    /**
     * @short Round to integer, away from zero
     * <pre>
     * y <- round(x)
     * </pre>
     */
    class Round : public ScalarFunction
    {
    public:
	Round ();
	double evaluate(std::vector<double const *> const &args) const;
	bool isDiscreteValued(std::vector<bool> const &mask) const;
    };
}

#endif /* FUNC_ROUND_H_ */
