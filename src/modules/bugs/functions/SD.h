#ifndef FUNC_SD_H_
#define FUNC_SD_H_

#include <function/VectorFunction.h>

namespace bugs {

    /**
     * @short Standard deviation function
     * SD calculates the standard deviation of the elements of an array.
     * The array must have at least 2 elements.
     * @see mean
     * <pre>
     * y <- sd(x[])
     * </pre>
     */
    class SD : public VectorFunction
    {
    public:
	SD ();
	void evaluate (double *x, std::vector<double const *> const &args,
		       std::vector<unsigned int> const &lengths) const;
	bool checkParameterLength (std::vector<unsigned int> const &len) const;
    };
}

#endif /* FUNC_SD_H_ */
