#include <config.h>
#include <Module.h>
#include <compiler/Compiler.h>
#include <function/InverseLinkFunc.h>
#include <function/Function.h>
#include <model/Model.h>
#include <algorithm>
#include <Console.h>

using std::vector;
using std::list;
using std::string;
using std::find;

Module::Module(string const &name)
    : _name(name)
{
    modules().push_back(this);
}

Module::~Module()
{
    list<Module*>::iterator p = find(modules().begin(), modules().end(), this);
    if (p != modules().end()) {
	modules().erase(p);
    }
}

void Module::insert(InverseLinkFunc *lfunc)
{
    _internal_link_functions.push_back(lfunc);
    _functions.push_back(lfunc);
}

void Module::insert(Function *func)
{
    _internal_functions.push_back(func);
    _functions.push_back(func);
}

void Module::insert(Distribution *dist)
{
    _distributions.push_back(dist);
}

void Module::insert(SamplerFactory *fac)
{
    _sampler_factories.push_back(fac);
}

void Module::insert(RNGFactory *fac)
{
    _rng_factories.push_back(fac);
}

void Module::insert(MonitorFactory *fac)
{
    _monitor_factories.push_back(fac);
}

void Module::load()
{
    for (unsigned int i = 0; i < _monitor_factories.size(); ++i) {
	Model::monitorFactories().push_front(_monitor_factories[i]);
    }
    for (unsigned int i = 0; i < _rng_factories.size(); ++i) {
	Model::rngFactories().push_front(_rng_factories[i]);
    }
    for (unsigned int i = 0; i < _sampler_factories.size(); ++i) {
	Model::samplerFactories().push_front(_sampler_factories[i]);
    }
    for (unsigned int i = 0; i < _distributions.size(); ++i) {
	Compiler::distTab().insert(_distributions[i]);
    }
    for (unsigned int i = 0; i < _internal_link_functions.size(); ++i) {
	Compiler::funcTab().insert(_internal_link_functions[i]);
    }
    for (unsigned int i = 0; i < _internal_functions.size(); ++i) {
	Compiler::funcTab().insert(_internal_functions[i]);
    }
}

void Module::unload()
{
    unsigned int i;
    for (i = 0; i < _internal_functions.size(); ++i) {
	Compiler::funcTab().erase(_internal_functions[i]);
    }
    for (i = 0; i < _internal_link_functions.size(); ++i) {
	Compiler::funcTab().erase(_internal_link_functions[i]);
    }
    for (i = 0; i < _distributions.size(); ++i) {
	Compiler::distTab().erase(_distributions[i]);
    }

    list<RNGFactory *> &rngf = Model::rngFactories();
    for (i = 0; i < _rng_factories.size(); ++i) {
	RNGFactory *f = _rng_factories[i];
	for (;;) {
	    /* A little paranoiac, in case someone inserted the
	       same sampler factory more than once */
	    list<RNGFactory *>::iterator p = find(rngf.begin(), rngf.end(), f);
	    if (p == rngf.end())
		break;
	    rngf.erase(p);
	}
    }

    list<SamplerFactory const*> &sf = Model::samplerFactories();
    for (i = 0; i < _sampler_factories.size(); ++i) {
	SamplerFactory *f = _sampler_factories[i];
	for (;;) {
	    /* A little paranoiac, in case someone inserted the
	       same sampler factory more than once */
	    list<SamplerFactory const*>::iterator p
		= find(sf.begin(), sf.end(), f);
	    if (p == sf.end())
		break;
	    sf.erase(p);
	}
    }
}

vector<Function*> const  &Module::functions() const
{
    return _functions;
}

vector<Distribution*> const  &Module::distributions() const
{
    return _distributions;
}

vector<SamplerFactory*> const  &Module::samplerFactories() const
{
    return _sampler_factories;
}

vector<RNGFactory*> const &Module::rngFactories() const
{
    return _rng_factories;
}

vector<MonitorFactory*> const &Module::monitorFactories() const
{
    return _monitor_factories;
}

string const &Module::name() const
{
    return _name;
}

list<Module *> &Module::modules()
{
    static list<Module*> _modules;
    return _modules;
}
