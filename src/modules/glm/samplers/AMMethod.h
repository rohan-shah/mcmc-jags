#ifndef AM_METHOD_H_
#define AM_METHOD_H_

#include "GLMMethod.h"

namespace glm {

    class AuxMix;

    class AMMethod : public GLMMethod {
	std::vector<AuxMix*> _aux;
      public:
	AMMethod(Updater const *updater, 
		     std::vector<Updater const *> const &sub_updaters,
		     unsigned int chain);
	~AMMethod();
	std::string name() const;
	double getPrecision(unsigned int i) const;
	double getValue(unsigned int i) const;
	void update(RNG *rng);
    };
    
}

#endif /* AM_METHOD_H_ */
