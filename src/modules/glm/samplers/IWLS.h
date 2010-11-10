#ifndef IWLS_H_
#define IWLS_H_

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
                              double *b, cholmod_sparse *A, bool &status);
    public:
	IWLS(GraphView const *view, 
	     std::vector<GraphView const *> const &sub_views,
	     unsigned int chain);
	std::string name() const;
	double getPrecision(unsigned int i) const;
	double getValue(unsigned int i) const;
	bool update(RNG *rng);
    };

}

#endif /* IWLS_H_ */
