#ifndef DPQ_FUNCTION_H_
#define DPQ_FUNCTION_H_

#include <function/ScalarFunction.h>

namespace bugs {

    class RScalarDist;

    class DPQFunction : public ScalarFunction
    {
	RScalarDist const *_dist;
    public:
	DPQFunction(std::string const &name, RScalarDist const *dist);
	/**
         * Returns the distribution from which the function derives its
         * value
         */
	RScalarDist const *dist() const;
	/**
         * Strips off the first argument and then checks that the remaining
         * arguments are valid parameters for the distribution
         */
	bool checkArgs(std::vector<double const *> const &args) const;
    };

}

#endif /* DPQ_FUNCTION_H_ */
