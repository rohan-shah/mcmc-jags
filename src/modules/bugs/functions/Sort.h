#ifndef FUNC_SORT_H_
#define FUNC_SORT_H_

#include <function/VectorFunction.h>

namespace bugs {

    /**
     * @short Sort function
     * Sorts the elements of a vector in ascending order
     * <pre>
     * y <- sort(x[])
     * </pre>
     */
    class Sort : public VectorFunction
    {
    public:
	Sort ();
	void evaluate(double *value, std::vector <double const *> const &args,
		      std::vector <unsigned int> const &lengths) const;
	unsigned int length(std::vector<unsigned int> const &parlengths) const;
	bool isDiscreteValued(std::vector<bool> const &mask) const;
    };
}

#endif /* FUNC_SORT_H_ */
