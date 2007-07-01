#ifndef FUNC_COS_H_
#define FUNC_COS_H_

#include <function/ScalarFunc.h>

/**
 * @short Cosine function
 * @see Sin
 * <pre>
 * y <- cos(x)
 * </pre>
 */
class Cos : public ScalarFunc
{
public:
    Cos ();
    double eval(std::vector<double const *> const &args) const;
};

#endif /* FUNC_COS_H_ */
