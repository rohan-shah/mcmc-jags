#ifndef TRANSPOSE_H_
#define TRANSPOSE_H_

#include <function/ArrayFunction.h>

namespace bugs {

    class Transpose : public ArrayFunction
    {
    public:
	Transpose();
	void evaluate(double *value, std::vector<double const *> const &args,
		      std::vector<std::vector<unsigned int> > const &dims) 
	    const;
	std::vector<unsigned int> 
	    dim(std::vector<std::vector<unsigned int> > const &dims) const;
	bool checkParameterDim(std::vector<std::vector<unsigned int> > const &dims) const;
	bool isScale(std::vector<bool> const &mask,
		     std::vector<bool> const &isfixed) const;
    };

}

#endif /* TRANSPOSE_H_ */
