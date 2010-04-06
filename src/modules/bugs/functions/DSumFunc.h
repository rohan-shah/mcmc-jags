#ifndef DSUM_FUNC_H_
#define DSUM_FUNC_H_

#include <function/ScalarFunction.h>

namespace bugs {
    
    /**
     * @short Sum of two discrete random variables
     */
    class DSumFunc : public ScalarFunction {
    public:
	DSumFunc();
	double evaluate(std::vector <double const *> const &args) const;
	bool isDiscreteValued(std::vector<bool> const &mask) const;
    };
    
}

#endif /* DSUM_FUNC_H_ */
