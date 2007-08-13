#ifndef FUNC_EXP_H_
#define FUNC_EXP_H_

#include <function/InverseLinkFunc.h>

/** 
 * @short Exponential function
 * @see Log
 * <pre>
 * y <- exp(x)
 * </pre>
 */
class Exp: public InverseLinkFunc
{
public:
    Exp ();
    double evaluateScalar(std::vector<double const *> const &args) const;
    double link(double mu) const;
    double gradLink(double mu) const;
};

#endif /* FUNC_EXP_H_ */
