#ifndef FUNC_STEP_H_
#define FUNC_STEP_H_

#include <function/ScalarFunction.h>

namespace bugs {

    /**
     *@short Step function
     * <pre>
     * y <- step(x)
     * y = 1 if x >= 0
     *     0 if x <  0
     * </pre>
     */
    class Step : public ScalarFunction
    {
    public:
	Step ();
	double evaluate(std::vector <double const *> const &args) const;
	bool isDiscreteValued(std::vector<bool> const &mask) const;
    };
    
}

#endif /* FUNC_STEP_H_ */
