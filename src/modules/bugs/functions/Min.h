#ifndef FUNC_MIN_H_
#define FUNC_MIN_H_

#include <function/VectorFunction.h>

namespace bugs {

    /**
     * @short Minimum element of a vector
     * @see Max
     */
    class Min: public VectorFunction
    {
    public:
	Min ();
	void evaluate(double *x, std::vector <double const *> const &args,
		      std::vector<unsigned int> const &lengths) const;
	bool checkParameterLength (std::vector<unsigned int> const &len) const;
	/** Returns true if all arguments are true */
	bool isDiscreteValued(std::vector<bool> const &mask) const;
    };
}

#endif /* FUNC_MIN_H_ */
