#ifndef IWLS_H_
#define IWLS_H_

extern "C" {
#include <cs.h>
}

#include "GLMMethod.h"



class LinkNode;

namespace glm {

    /**
     * @short Iteratively Weighted Least Squares
     */
    class IWLS : public GLMMethod {
	std::vector<LinkNode const *> _link;
	std::vector<GLMFamily> _family;
	bool _init;
	double _w;
        double var(unsigned int i) const;
        double logPTransition(std::vector<double> const &xorig,
                              std::vector<double> const &x,
                              double const *b, cs const *A);
    public:
	IWLS(Updater const *updater, 
	     std::vector<Updater const *> const &sub_updaters,
	     unsigned int chain);
	std::string name() const;
	double getPrecision(unsigned int i) const;
	double getValue(unsigned int i) const;
	void update(RNG *rng);
    };

}
#endif /* GLM_METHOD_H_ */
