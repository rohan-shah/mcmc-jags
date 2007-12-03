#ifndef DIRICHLET_METROPOLIS_H_
#define DIRICHLET_METROPOLIS_H_

#include <sampler/RWMetropolis.h>

class DirchMetropolis : public RWMetropolis
{
public:
    DirchMetropolis(StochasticNode* node);
    void transform(double const *v, unsigned int length,
		   double *nv, unsigned int nlength) const;
    void untransform(double const *nv, unsigned int nlength,
		     double *v, unsigned int length) const;
    std::string name() const;
};

#endif /* DIRICHLET_METROPOLIS_H_ */
