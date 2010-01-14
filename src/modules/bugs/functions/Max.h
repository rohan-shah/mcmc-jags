#ifndef FUNC_MAX_H_
#define FUNC_MAX_H_

#include <function/VectorFunction.h>

namespace bugs {
    
    /**
     * @short Maximum element of a vector
     * @see Min
     */
    class Max: public VectorFunction
    {
    public:
	Max ();
	void evaluate(double *x, std::vector <double const *> const &args,
		      std::vector<unsigned int> const &lengths) const;
	bool checkParameterLength (std::vector<unsigned int> const &len) const;
	/** Returns true if all arguments are true */
	bool isDiscreteValued(std::vector<bool> const &mask) const;
    };

}

#endif /* FUNC_MAX_H_ */
