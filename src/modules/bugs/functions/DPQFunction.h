#ifndef DPQ_FUNCTION_H_
#define DPQ_FUNCTION_H_

#include <function/ScalarFunction.h>

class DistScalarRmath;

namespace bugs {

    class DPQFunction : public ScalarFunction
    {
	DistScalarRmath *_dist;
	std::vector<std::vector<unsigned int> > _dims;
    public:
	DPQFunction(std::string const &name, DistScalarRmath *dist);
	~DPQFunction();
	/**
         * Returns the distribution from which the function derives its
         * value
         */
	DistScalarRmath const *dist() const;
	/**
         * Strips off the first argument and then checks that the remaining
         * arguments are valid parameters for the distribution
         */
	bool checkArgs(std::vector<double const *> const &args) const;
    };

}

#endif /* DPQ_FUNCTION_H_ */
