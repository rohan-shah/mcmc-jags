#ifndef BASE_RNG_FACTORY_H_
#define BASE_RNG_FACTORY_H_

#include <rng/RNGFactory.h>

namespace base {
    
/**
 * @short Factory object for Base Random Number Generators
 */
    class BaseRNGFactory : public RNGFactory
    {
	unsigned int _index;
	unsigned int _seed;
	std::vector<RNG*> _rngvec;
    public:
	BaseRNGFactory();
	~BaseRNGFactory();
	std::vector<RNG *> makeRNGs(unsigned int n);
	RNG * makeRNG(std::string const &name);
	std::string name() const;
    };

}

#endif /* BASE_RNG_FACTORY_H_ */
