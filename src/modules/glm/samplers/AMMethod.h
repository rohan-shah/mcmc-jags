#ifndef AM_METHOD_H_
#define AM_METHOD_H_

#include "GLMMethod.h"

namespace glm {

    class AuxMix;

    class AMMethod : public GLMMethod {
	std::vector<AuxMix*> _aux;
      public:
	AMMethod(GraphView const *view, 
		 std::vector<GraphView const *> const &sub_views,
		 unsigned int chain);
	~AMMethod();
	std::string name() const;
	double getPrecision(unsigned int i) const;
	double getValue(unsigned int i) const;
	void update(RNG *rng);
    };
    
}

#endif /* AM_METHOD_H_ */
