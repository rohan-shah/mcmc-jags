#ifndef FUNC_MEAN_H_
#define FUNC_MEAN_H_

#include <function/Function.h>

/**
 * @short Mean function
 * Mean calculates the mean of the elements of an array
 * @see SD
 * <pre>
 * y <- mean(x[])
 * </pre>
 */
class Mean : public Function
{
public:
    Mean ();
    void evaluate(double *x, std::vector<double const *> const &args,
		  std::vector<std::vector<unsigned int> > const &dims) const;
    bool checkParameterDim (std::vector<std::vector<unsigned int> > const &dims) const;
    bool isLinear(std::vector<bool> const &mask,
		  std::vector<bool> const &fix) const;
    bool isScale(unsigned int index, std::vector<bool> const &fix) const;
};

#endif /* FUNC_MEAN_H_ */
