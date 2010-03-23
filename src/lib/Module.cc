#include <config.h>
#include <Module.h>
#include <compiler/Compiler.h>
#include <model/Model.h>
#include <algorithm>
#include <Console.h>

using std::vector;
using std::list;
using std::string;
using std::find;
using std::pair;

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

/*
void Module::insert(Distribution *dist)
{
    _distributions.push_back(dist);
}
*/

/*
void Module::insert(DistPtr const &dp)
{
    if (!isNULL(dp)) {
	_distributions.push_back(DIST(dp));
	_dp_list.push_back(dp);
    }
}



void Module::insert(FunctionPtr const &fp)
{
    if (!isNULL(fp)) {
	_functions.push_back(FUNC(fp));
	_fp_list.push_back(fp);
    }
}

void Module::insert(DistPtr const &dp, FunctionPtr const &fp)
{
    _obs_functions.push_back(pair<DistPtr,FunctionPtr>(dp, fp));
    insert(dp);
    insert(fp);
}
*/

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

//


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
	Compiler::distTab().insert(_dp_list[i]);
    }
    for (unsigned int i = 0; i < _fp_list.size(); ++i) {
	Compiler::funcTab().insert(_fp_list[i]);
    }
    for (unsigned int i = 0; i < _obs_functions.size(); ++i) {
	Compiler::obsFuncTab().insert(_obs_functions[i].first,
				      _obs_functions[i].second);
    }
}

void Module::unload()
{
    unsigned int i;
    
    for (i = 0; i < _obs_functions.size(); ++i) {
	Compiler::obsFuncTab().erase(_obs_functions[i].first,
				     _obs_functions[i].second);
    }
    for (i = 0; i < _fp_list.size(); ++i) {
	Compiler::funcTab().erase(_fp_list[i]);
    }
    for (i = 0; i < _distributions.size(); ++i) {
	Compiler::distTab().erase(_dp_list[i]);
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
    static list<Module*> _modules;
    return _modules;
}
