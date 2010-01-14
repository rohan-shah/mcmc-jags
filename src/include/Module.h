#ifndef _MODULE_H_
#define _MODULE_H_

#include <vector>
#include <string>
#include <list>
#include <utility>

#include <function/FunctionPtr.h>

class Distribution;
class SamplerFactory;
class RNGFactory;
class MonitorFactory;
class InverseLinkFunction;
class ScalarFunction;
class VectorFunction;
class ArrayFunction;

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
    std::string _name;
    std::vector<FunctionPtr> _fp_list;
    std::vector<Function*> _functions;
    std::vector<std::pair<Distribution*, FunctionPtr> > _obs_functions;
    std::vector<Distribution*> _distributions;
    std::vector<SamplerFactory*> _sampler_factories;
    std::vector<RNGFactory*> _rng_factories;
    std::vector<MonitorFactory*> _monitor_factories;
public:
    Module(std::string const &name);
    virtual ~Module();
    void insert(ScalarFunction*);
    void insert(InverseLinkFunc*);
    void insert(VectorFunction*);
    void insert(ArrayFunction*);
    void insert(Distribution*);
    void insert(Distribution*, ScalarFunction*);
    void insert(Distribution*, InverseLinkFunc*);
    void insert(Distribution*, VectorFunction*);
    void insert(Distribution*, ArrayFunction*);
    void insert(SamplerFactory*);
    void insert(RNGFactory*);
    void insert(MonitorFactory*);
    std::vector<Function*> const &functions() const;
    std::vector<Distribution*> const &distributions() const;
    std::vector<SamplerFactory*> const &samplerFactories() const;
    std::vector<RNGFactory*> const &rngFactories() const;
    std::vector<MonitorFactory*> const &monitorFactories() const;
    void load();
    void unload();
    std::string const &name() const;
    static std::list<Module *> &modules();
};

#endif /* _MODULE_H_ */
