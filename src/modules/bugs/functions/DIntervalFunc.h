#ifndef FUNC_DINTERVAL_H_
#define FUNC_DINTERVAL_H_

#include <function/VectorFunction.h>

namespace bugs {

    /**
     * @short Function version of dinterval distribution
     */
    class DIntervalFunc: public VectorFunction
    {
    public:
	DIntervalFunc ();
	void evaluate (double *value,
		       std::vector <double const *> const &args,
		       std::vector<unsigned int> const &lengths) const;
	bool checkParameterLength (std::vector<unsigned int> const &args) const;
	bool isDiscreteValued(std::vector<bool> const &mask) const;
    };

}

#endif /* FUNC_DINTERVAL_H_ */
