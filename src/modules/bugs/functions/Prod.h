#ifndef FUNC_PROD_H_
#define FUNC_PROD_H_

#include <function/Function.h>

namespace bugs {

    /**
     * @short Product of an array
     * Sum calculates the product of the elements of an array
     * @see Sum
     * <pre>
     * y <- prod(x[])
     * </pre>
     */
    class Prod : public Function
    {
    public:
	Prod ();
	void evaluate(double *x, std::vector <double const *> const &args,
		      std::vector<unsigned int> const &lengths,
		      std::vector<std::vector<unsigned int> > const &dims)
	    const;
	bool checkParameterDim(std::vector<std::vector<unsigned int> > const &dims)
	    const;
	bool isDiscreteValued(std::vector<bool> const &mask) const;
    };

}

#endif /* FUNC_PROD_H_ */
