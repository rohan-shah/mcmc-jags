#ifndef _SUPER_DUPER_RNG_H_
#define _SUPER_DUPER_RNG_H_

#include <rng/RmathRNG.h>

namespace base {

    class SuperDuperRNG : public RmathRNG
    {
	unsigned int I[2];
	void fixupSeeds();
    public:
	SuperDuperRNG(unsigned int seed, NormKind norm_kind);
	double uniform();
	void init(unsigned int seed);
	bool setState(std::vector<int> const &state);
	void getState(std::vector<int> &state) const;
    };

}

#endif /* _SUPER_DUPER_RNG_H_ */
