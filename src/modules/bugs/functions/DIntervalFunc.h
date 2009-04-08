#ifndef FUNC_DINTERVAL_H_
#define FUNC_DINTERVAL_H_

#include <function/Function.h>

namespace bugs {

    /**
     * @short Function version of dinterval distribution
     */
    class DIntervalFunc: public Function
    {
    public:
	DIntervalFunc ();
	void evaluate (double *value,
		       std::vector <double const *> const &args,
		       std::vector<unsigned int> const &lengths,
		       std::vector<std::vector<unsigned int> > const &dims) 
	    const;
	bool checkParameterDim (std::vector <std::vector<unsigned int> > const &dims) const;
	bool isDiscreteValued(std::vector<bool> const &mask) const;
    };

}

#endif /* FUNC_DINTERVAL_H_ */
