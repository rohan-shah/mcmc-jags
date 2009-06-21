#ifndef FUNC_SUM_H_
#define FUNC_SUM_H_

#include <function/Function.h>

namespace bugs {

    /**
     * @short Sum of an array
     * Sum calculates the sum of the elements of an array
     * @see Mean
     * <pre>
     * y <- sum(x[])
     * </pre>
     */
    class Sum : public Function
    {
    public:
	Sum ();
	void evaluate(double *x, std::vector <double const *> const &args,
		      std::vector<unsigned int> const &lengths,
		      std::vector<std::vector<unsigned int> > const &dims) 
	    const;
	bool checkParameterDim(std::vector<std::vector<unsigned int> > const &dims)
	    const;
	bool isDiscreteValued(std::vector<bool> const &mask) const;
	bool isScale(std::vector<bool> const &mask,
		      std::vector<bool> const &isfixed) const;
    };
    
}

#endif /* FUNC_SUM_H_ */
