#ifndef FUNC_LOGIT_H_
#define FUNC_LOGIT_H_

#include <function/ScalarFunc.h>

namespace bugs {

    /**
     * @short Logistic function
     * @see ILogit
     * <pre>
     * y <- logit(p)
     * y = log(p/(1-p)) for 0 < p < 1
     * </pre>
     */
    class Logit:public ScalarFunc
    {
    public:
	Logit();
	double evaluateScalar(std::vector <double const *> const &args) const;
	bool checkScalarValue(std::vector<double const *> const &args) const;
    };

}

#endif /* FUNC_LOGIT_H_ */
