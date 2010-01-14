#ifndef FUNC_POW_H_
#define FUNC_POW_H_

#include <function/ScalarFunction.h>

namespace bugs {

    /**
     * @short Power function
     * If y <- pow(x,z) and x is negative, then z must be an integer
     * <pre>
     * y <- pow(x,z)
     * y = x^z
     * </pre>
     */
    class Pow : public ScalarFunction
    {
    public:
	Pow ();
	double evaluate(std::vector<double const *> const &args) const;
	bool checkScalarValue(std::vector<double const*> const &args) const;
	bool isPower(std::vector<bool> const &mask, 
		     std::vector<bool> const &fix) const;
    };

}

#endif /* FUNC_POW_H_ */
