#ifndef MEXP_H_
#define MEXP_H_

#include <function/ArrayFunction.h>

namespace msm {

class Mexp : public ArrayFunction
{
public:
    Mexp();
    void evaluate (double *value, std::vector<double const *> const &args,
		   std::vector<std::vector<unsigned int> > const &dims) const;
    std::vector<unsigned int> 
	dim(std::vector<std::vector<unsigned int> > const &dims) const;
    bool checkParameterDim(std::vector <std::vector<unsigned int> > const &dims) const;
};

}

#endif /* MEXP_H_ */
