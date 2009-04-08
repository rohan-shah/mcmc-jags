#ifndef DSUM_FUNC_H_
#define DSUM_FUNC_H_

#include <function/ScalarFunc.h>

namespace bugs {
    
    /**
     * @short Sum of two discrete random variables
     */
    class DSumFunc : public ScalarFunc {
    public:
	DSumFunc();
	double evaluateScalar(std::vector <double const *> const &args) const;
    };
    
}

#endif /* DSUM_FUNC_H_ */
