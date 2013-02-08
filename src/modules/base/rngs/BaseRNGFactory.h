#ifndef BASE_RNG_FACTORY_H_
#define BASE_RNG_FACTORY_H_

#include <rng/RNGFactory.h>

namespace jags {
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
	void setSeed(unsigned int seed);
	std::vector<RNG *> makeRNGs(unsigned int n);
	RNG * makeRNG(std::string const &name);
	std::string name() const;
    };

}}

#endif /* BASE_RNG_FACTORY_H_ */
