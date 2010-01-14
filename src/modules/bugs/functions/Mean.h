#ifndef FUNC_MEAN_H_
#define FUNC_MEAN_H_

#include <function/VectorFunction.h>

namespace bugs {

    /**
     * @short Mean function
     * Mean calculates the mean of the elements of an array
     * @see SD
     * <pre>
     * y <- mean(x[])
     * </pre>
     */
    class Mean : public VectorFunction
    {
    public:
	Mean ();
	void evaluate(double *x, std::vector<double const *> const &args,
		      std::vector<unsigned int> const &dims) const;
	bool checkParameterLength (std::vector<unsigned int> const &len) const;
	bool isScale(std::vector<bool> const &mask,
		     std::vector<bool> const &fix) const;
    };
}

#endif /* FUNC_MEAN_H_ */
