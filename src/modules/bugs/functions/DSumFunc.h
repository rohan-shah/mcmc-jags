#ifndef DSUM_FUNC_H_
#define DSUM_FUNC_H_

#include <function/ArrayFunction.h>

namespace bugs {
    
    /**
     * @short Sum of two discrete random variables
     */
    class DSumFunc : public ArrayFunction {
    public:
	DSumFunc();
	void evaluate(double *x,
		      std::vector <double const *> const &args,
		      std::vector<std::vector<unsigned int> > const &dims) 
	    const;
	bool checkParameterDim(std::vector<std::vector<unsigned int> > const 
			       &dims) const;
	std::vector<unsigned int> 
	    dim(std::vector <std::vector<unsigned int> > const &dims) const;
	bool isDiscreteValued(std::vector<bool> const &mask) const;
    };
    
}

#endif /* DSUM_FUNC_H_ */
