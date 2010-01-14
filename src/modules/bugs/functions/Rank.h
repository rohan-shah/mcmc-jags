#ifndef FUNC_RANK_H_
#define FUNC_RANK_H_

#include <function/VectorFunction.h>

namespace bugs {

    /**
     * @short Sort function
     * Calculates the ranks of the elements of a vector in ascending order
     * <pre>
     * y <- sort(x[])
     * </pre>
     */
    class Rank : public VectorFunction
    {
    public:
	Rank ();
	void evaluate(double *value, 
		      std::vector<double const *> const &args,
		      std::vector<unsigned int> const &lengths) const;
	unsigned int length(std::vector<unsigned int> const &args) const;
	bool isDiscreteValued(std::vector<bool> const &mask) const;
    };
    
}

#endif /* FUNC_RANK_H_ */
