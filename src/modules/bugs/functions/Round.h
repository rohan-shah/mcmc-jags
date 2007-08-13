#ifndef FUNC_ROUND_H_
#define FUNC_ROUND_H_

#include <function/ScalarFunc.h>

/**
 * @short Round to integer, away from zero
 * <pre>
 * y <- round(x)
 * </pre>
 */
class Round : public ScalarFunc
{
public:
  Round ();
  double evaluateScalar(std::vector<double const *> const &args) const;
};

#endif /* FUNC_ROUND_H_ */
