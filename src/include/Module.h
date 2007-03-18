#ifndef _MODULE_H_
#define _MODULE_H_

#include <vector>

class InverseLinkFunc;
class Function;
class Distribution;
class SamplerFactory;
class RNGFactory;

/**
 * @short Memory management for dynamically loadable modules
 *
 * Dynamically loadable modules may extend the capabilities of the JAGS library
 * by defining new subclasses of Function, Distribution, SamplerFactory, 
 * and RNGFactory. 
 *
 * Each module must instantiate a subclass of Module. The constructor for
 * this subcluass must dynamically allocate instances of the sub-classes 
 * defined by the module, and store them with the appropriate insert 
 * member function.
 */
class Module {
    std::vector<Function*> _functions;
    std::vector<InverseLinkFunc*> _link_functions;
    std::vector<Distribution*> _distributions;
    std::vector<SamplerFactory*> _sampler_factories;
    std::vector<RNGFactory*> _rng_factories;
public:
    Module();
    virtual ~Module();
    void insert(InverseLinkFunc*);
    void insert(Function*);
    void insert(Distribution*);
    void insert(SamplerFactory*);
    void insert(RNGFactory*);
    std::vector<Function*> const &functions() const;
    std::vector<Distribution*> const &distributions() const;
    std::vector<SamplerFactory*> const &samplerFactories() const;
    std::vector<RNGFactory*> const &rngFactories() const;
};

#endif /* _MODULE_H_ */
