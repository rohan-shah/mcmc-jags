#ifndef FUNC_TRUNC_H_
#define FUNC_TRUNC_H_

#include <function/ScalarFunc.h>

/**
 * @short Round to integer, towards zero
 * <pre>
 * y <- trunc(x)
 * </pre>
 */
class Trunc : public ScalarFunc
{
public:
  Trunc ();
  double eval(std::vector<double const *> const &args) const;
};

#endif /* FUNC_TRUNC_H_ */
