#include <config.h>
#include <Module.h>
#include <compiler/Compiler.h>
#include <model/Model.h>
#include <algorithm>

using std::vector;
using std::list;
using std::string;
using std::find;
using std::pair;

Module::Module(string const &name)
    : _name(name), _loaded(false)
{
    modules().push_back(this);
}

Module::~Module()
{
    //FIXME: Could be causing windows segfault??
    unload();
    list<Module*>::iterator p = find(modules().begin(), modules().end(), this);
    if (p != modules().end()) {
	modules().erase(p);
    }
}

void Module::insert(ScalarFunction *func)
{
    _functions.push_back(func);
    _fp_list.push_back(FunctionPtr(func));
}

void Module::insert(LinkFunction *func)
{
    _functions.push_back(func);
    _fp_list.push_back(FunctionPtr(func));

}

void Module::insert(VectorFunction *func)
{
    _functions.push_back(func);
    _fp_list.push_back(FunctionPtr(func));

}

void Module::insert(ArrayFunction *func)
{
    _functions.push_back(func);
    _fp_list.push_back(FunctionPtr(func));

}

void Module::insert(ScalarDist *dist)
{
    _distributions.push_back(dist);
    _dp_list.push_back(DistPtr(dist));
}

void Module::insert(VectorDist *dist)
{
    _distributions.push_back(dist);
    _dp_list.push_back(DistPtr(dist));

}

void Module::insert(ArrayDist *dist)
{
    _distributions.push_back(dist);
    _dp_list.push_back(DistPtr(dist));

}

void Module::insert(ScalarDist *dist, ScalarFunction *func)
{
    _obs_functions.push_back(pair<DistPtr,FunctionPtr>(dist,func));
    insert(dist);
    insert(func);
}

void Module::insert(ScalarDist *dist, LinkFunction *func)
{
    _obs_functions.push_back(pair<DistPtr,FunctionPtr>(dist,func));
    insert(dist);
    insert(func);
}

void Module::insert(ScalarDist *dist, VectorFunction *func)
{
    _obs_functions.push_back(pair<DistPtr,FunctionPtr>(dist,func));
    insert(dist);
    insert(func);
}

void Module::insert(ScalarDist *dist, ArrayFunction *func)
{
    _obs_functions.push_back(pair<DistPtr,FunctionPtr>(dist,func));
    insert(dist);
    insert(func);
}

//

void Module::insert(VectorDist *dist, ScalarFunction *func)
{
    _obs_functions.push_back(pair<DistPtr,FunctionPtr>(dist,func));
    insert(dist);
    insert(func);
}

void Module::insert(VectorDist *dist, LinkFunction *func)
{
    _obs_functions.push_back(pair<DistPtr,FunctionPtr>(dist,func));
    insert(dist);
    insert(func);
}

void Module::insert(VectorDist *dist, VectorFunction *func)
{
    _obs_functions.push_back(pair<DistPtr,FunctionPtr>(dist,func));
    insert(dist);
    insert(func);
}

void Module::insert(VectorDist *dist, ArrayFunction *func)
{
    _obs_functions.push_back(pair<DistPtr,FunctionPtr>(dist,func));
    insert(dist);
    insert(func);
}

//

void Module::insert(ArrayDist *dist, ScalarFunction *func)
{
    _obs_functions.push_back(pair<DistPtr,FunctionPtr>(dist,func));
    insert(dist);
    insert(func);
}

void Module::insert(ArrayDist *dist, LinkFunction *func)
{
    _obs_functions.push_back(pair<DistPtr,FunctionPtr>(dist,func));
    insert(dist);
    insert(func);
}

void Module::insert(ArrayDist *dist, VectorFunction *func)
{
    _obs_functions.push_back(pair<DistPtr,FunctionPtr>(dist,func));
    insert(dist);
    insert(func);
}

void Module::insert(ArrayDist *dist, ArrayFunction *func)
{
    _obs_functions.push_back(pair<DistPtr,FunctionPtr>(dist,func));
    insert(dist);
    insert(func);
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
    if (_loaded)
	return;

    for (unsigned int i = 0; i < _monitor_factories.size(); ++i) {
	pair<MonitorFactory*,bool> p(_monitor_factories[i], true);
	Model::monitorFactories().push_front(p);
    }
    for (unsigned int i = 0; i < _rng_factories.size(); ++i) {
	pair<RNGFactory*, bool> p(_rng_factories[i], true);
	Model::rngFactories().push_front(p);
    }
    for (unsigned int i = 0; i < _sampler_factories.size(); ++i) {
	pair<SamplerFactory*, bool> p(_sampler_factories[i], true);
	Model::samplerFactories().push_front(p);
    }
    for (unsigned int i = 0; i < _dp_list.size(); ++i) {
	Compiler::distTab().insert(_dp_list[i]);
    }
    for (unsigned int i = 0; i < _fp_list.size(); ++i) {
	Compiler::funcTab().insert(_fp_list[i]);
    }
    for (unsigned int i = 0; i < _obs_functions.size(); ++i) {
	Compiler::obsFuncTab().insert(_obs_functions[i].first,
				      _obs_functions[i].second);
    }

    _loaded = true;
    loadedModules().push_back(this);
}

void Module::unload()
{
    if (!_loaded)
	return;

    loadedModules().remove(this);
    _loaded = false;

    for (unsigned int i = 0; i < _fp_list.size(); ++i) {
	Compiler::funcTab().erase(_fp_list[i]);
    }
    for (unsigned int i = 0; i < _obs_functions.size(); ++i) {
	Compiler::obsFuncTab().erase(_obs_functions[i].first,
				     _obs_functions[i].second);
    }
    for (unsigned int i = 0; i < _distributions.size(); ++i) {
	Compiler::distTab().erase(_dp_list[i]);
    }

    list<pair<RNGFactory *, bool> > &rngf = Model::rngFactories();
    for (unsigned int i = 0; i < _rng_factories.size(); ++i) {
	RNGFactory *f = _rng_factories[i];
	rngf.remove(pair<RNGFactory *, bool>(f, true));
	rngf.remove(pair<RNGFactory *, bool>(f, false));
    }

    list<pair<SamplerFactory *, bool> > &sf = Model::samplerFactories();
    for (unsigned int i = 0; i < _sampler_factories.size(); ++i) {
	SamplerFactory *f = _sampler_factories[i];
	sf.remove(pair<SamplerFactory *, bool>(f, true));
	sf.remove(pair<SamplerFactory *, bool>(f, false));
    }

    list<pair<MonitorFactory *, bool> > &mf = Model::monitorFactories();
    for (unsigned int i = 0; i < _monitor_factories.size(); ++i) {
	MonitorFactory *f = _monitor_factories[i];
	mf.remove(pair<MonitorFactory *, bool>(f, true));
	mf.remove(pair<MonitorFactory *, bool>(f, false));
    }

}

vector<Function*> const &Module::functions() const
{
    return _functions;
}

vector<Distribution*> const &Module::distributions() const
{
    return _distributions;
}

vector<SamplerFactory*> const &Module::samplerFactories() const
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
    static list<Module*> *_modules = new list<Module*>;
    return *_modules;
}

list<Module *> &Module::loadedModules()
{
    static list<Module*> *_modules = new list<Module*>;
    return *_modules;
}

