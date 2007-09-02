#include <config.h>
#include <Console.h>
#include <compiler/Compiler.h>
#include <compiler/parser_extra.h>
#include <compiler/ParseTree.h>
//#include <model/TraceMonitor.h>
#include <model/Monitor.h>
#include <model/BUGSModel.h>
#include <graph/NodeError.h>
#include <sampler/SamplerFactory.h>
#include <graph/Node.h>
#include <sarray/Range.h>
#include <sarray/SArray.h>
#include <rng/RNG.h>
#include <sarray/util.h>

#include <map>
#include <list>
#include <stdexcept>
#include <fstream>
#include <vector>

using std::ostream;
using std::ofstream;
using std::endl;
using std::flush;
using std::vector;
using std::string;
using std::map;
using std::list;
using std::set;
using std::pair;
using std::FILE;

/* Helper functions for dumpState */

static bool isData(Node const *node)
{
  return node->isVariable() && node->isObserved();
}

static bool isParameter(Node const *node)
{
  return node->isVariable() && !node->isObserved();
}

static bool alwaysTrue(Node const *node)
{
  return true;
}

Console::Console(ostream &out, ostream &err)
  : _out(out), _err(err), _model(0), _pdata(0), _prelations(0), 
    _pvariables(0)
{
}

Console::~Console()
{
  delete _model;
  delete _pdata;
  delete _prelations;
  if (_pvariables) {
      for (unsigned int i = 0; i < _pvariables->size(); ++i) {
	  delete (*_pvariables)[i];
      }
      delete _pvariables;
  }
}

bool Console::checkModel(FILE *file)
{
    if (_model) {
	_out << "Replacing existing model" << endl;
	clearModel();
    }
    _model = 0;
    int lineno =  parse_bugs(file, &_pvariables, &_pdata, &_prelations);
    if (lineno != 0) {
	_err << endl << "Parse error on line " << lineno << endl;
	clearModel();
	return false;
    }
    return true;
}


bool Console::compile(map<string, SArray> &data_table, unsigned int nchain,
                      bool gendata)
{
  if (nchain == 0) {
    _err << "You must have at least one chain" << endl;
    return false;
  }
  
  RNG *datagen_rng = 0;
  if (_pdata && gendata) {
    BUGSModel datagen_model(1);
    Compiler compiler(datagen_model, data_table);
    _out << "Compiling data graph" << endl;
    try {
      if (_pvariables) {
	_out << "   Declaring variables" << endl;
	compiler.declareVariables(*_pvariables);
      }
      _out << "   Resolving undeclared variables" << endl;
      compiler.undeclaredVariables(_pdata);
      _out << "   Allocating nodes" << endl;
      compiler.writeRelations(_pdata);
      
      /* Check validity of data generating model */
      vector<Node*> nodes;
      datagen_model.graph().getNodes(nodes);
      for (unsigned int i = 0; i < nodes.size(); ++i) {
	if (nodes[i]->isObserved()) {
	  vector<Node const*> const &parents = nodes[i]->parents();
	  for (vector<Node const*>::const_iterator p = parents.begin();
	       p != parents.end(); ++p)
	    {
	      if (!((*p)->isObserved())) {
		_err << "Invalid data graph: observed node " 
		     << nodes[i]->name(datagen_model.symtab()) 
		     << " has unobserved parent " 
		     << (*p)->name(datagen_model.symtab()) 
		     << "\n";
		return false;
	      }
	    }
	}
      }
      _out << "   Initializing" << endl;
      datagen_model.initialize(true);
      // Do a single update (by forward sampling)
      datagen_model.update(1);
      //Save data generating RNG for later use
      datagen_rng = datagen_model.rng(0);
      _out << "   Reading data back into data table" << endl;
      datagen_model.symtab().readValues(data_table, 0, alwaysTrue);
    }
    catch (NodeError except) {
      _err << "Error in node " << 
	datagen_model.symtab().getName(except.node) << "\n";
      _err << except.what() << endl;
      return false;
    }
    catch (std::runtime_error except) {
      _err << "RUNTIME ERROR:\n";
      _err << except.what() << endl;
      return false;
    }
    catch (std::logic_error except) {
      _err << "LOGIC ERROR:\n";
      _err << except.what() << '\n';
      _err << "Please send a bug report to " << PACKAGE_BUGREPORT << endl;
      return false;
    }
  }

  _model = new BUGSModel(nchain);
  Compiler compiler(*_model, data_table);

  _out << "Compiling model graph" << endl;
  try {
    if (_pvariables) {
      _out << "   Declaring variables" << endl;
      compiler.declareVariables(*_pvariables);
    }
    if (_prelations) {
       _out << "   Resolving undeclared variables" << endl;
       compiler.undeclaredVariables(_prelations);
       _out << "   Allocating nodes" << endl;
       compiler.writeRelations(_prelations);
     }
     else {
       _err << "Nothing to compile" << endl;
       return false;
     }
     if (_model) {
       _out << "   Graph Size: " << _model->graph().size() << endl;
       if (datagen_rng) {
          // Reuse the data-generation RNG, if there is one, for chain 0 
          _model->setRNG(datagen_rng, 0);
       }
     }
     else {
       _err << "No model" << endl;
       return false;
     }
  }
  catch (NodeError except) {
    _err << "Error in node " << _model->symtab().getName(except.node) << '\n';
    _err << except.what() << endl;
    clearModel();
    return false;
  }
  catch (std::runtime_error except) {
    _err << "RUNTIME ERROR:\n";
    _err << except.what() << endl;
    clearModel();
    return false;
  }
  catch (std::logic_error except) {
    _err << "LOGIC ERROR:\n";
    _err << except.what() << '\n';
    _err << "Please send a bug report to " << PACKAGE_BUGREPORT << endl;
    clearModel();
    return false;
  }
  return true;
}

bool Console::initialize()
{
    if (_model == 0) {
	_err << "Can't initialize. No model!" << endl;
	return false;
    }
    if (_model->graph().size() == 0) {
	_err << "Can't initialize. No nodes in graph (Have you compiled the model?)" 	 << endl;
	return false;
    }
    try {
	_model->initialize(false);
	_model->addDevianceNode();
    }
    catch (NodeError except) {
	_err << "Error in node " << _model->symtab().getName(except.node) << endl;
	_err << except.what() << endl;
	return false;
    }
    catch (std::runtime_error except) {
	_err << "RUNTIME ERROR" << endl;
	_err << except.what() << endl;
	return false;
    }
    catch (std::logic_error except) {
	_err << "LOGIC ERROR" << endl;
	_err << except.what() << endl;
	_err << "Please send a bug report to " << PACKAGE_BUGREPORT << endl;
	return false;
    }
    return true;
}

bool Console::setParameters(map<string, SArray> const &init_table,
			    unsigned int chain)
{
  if (_model == 0) {
    _err << "Can't set initial values. No model!" << endl;    
    return false;
  }
  if (chain == 0 || chain > nchain()) {
    _err << "Invalid chain number" << endl;
    return false;
  }

  try {
    _model->setParameters(init_table, chain - 1);
  }
  catch (NodeError except) {
    _err << "Error in node " << _model->symtab().getName(except.node) << '\n';
    _err << except.what() << endl;
    clearModel();
    return false;
  }
  catch (std::runtime_error except) {
    _err << "RUNTIME ERROR:\n";
    _err << except.what() << endl;
    clearModel();
    return false;
  }
  catch (std::logic_error except) {
    _err << "LOGIC ERROR\n";
    _err << except.what() << '\n';
    _err << "Please send a bug report to " << PACKAGE_BUGREPORT << endl;
    clearModel();
    return false;
  }
  return true;
}

bool Console::setRNGname(string const &name, unsigned int chain)
{
  if (_model == 0) {
    _err << "Can't set RNG name. No model!" << endl;    
    return false;
  }
  try {
    bool name_ok = _model->setRNG(name, chain - 1);
    if (!name_ok) {
       _err << "WARNING: RNG name " << name << " not found\n";
    }
  }
  catch (NodeError except) {
    _err << "Error in node " << _model->symtab().getName(except.node) << '\n';
    _err << except.what() << endl;
    clearModel();
    return false;
  }
  catch (std::runtime_error except) {
    _err << "RUNTIME ERROR:\n";
    _err << except.what() << endl;
    clearModel();
    return false;
  }
  catch (std::logic_error except) {
    _err << "LOGIC ERROR\n";
    _err << except.what() << '\n';
    _err << "Please send a bug report to " << PACKAGE_BUGREPORT << endl;
    clearModel();
    return false;
  }
  return true;
}

bool Console::update(unsigned int n)
{
    if (_model == 0) {
	_err << "Can't update. No model!" << endl;    
	return false;
    }
    if (!_model->isInitialized()) {
	_err << "Model not initialized" << endl;
	return false;
    }
    try {
	_model->update(n);
    }
    catch (NodeError except) {
	_err << "Error in node " << _model->symtab().getName(except.node) << '\n';
	_err << except.what() << endl;
	clearModel();
	return false;
    }
    catch (std::runtime_error except) {
	_err << "RUNTIME ERROR:\n";
	_err << except.what() << endl;
	clearModel();
	return false;
    }
    catch (std::logic_error except) {
	_err << "LOGIC ERROR:\n";
	_err << except.what() << '\n';
	_err << "Please send a bug report to " << PACKAGE_BUGREPORT << endl;
	clearModel();
	return false;
    }
    return true;
}

unsigned int Console::iter() const
{
  if (!_model) {
    return 0;
  }
  else {
    return _model->iteration();
  }
}

bool Console::setMonitor(string const &name, Range const &range,
			 unsigned int thin, string const &type)
{
    if (!_model) {
	_err << "Can't set monitor. No model!" << endl;    
	return false;
    }

    try {
	bool ok = _model->setMonitor(name, range, thin, type);
	if (!ok) {
	    _err << "Failed to set " << type << " monitor for node " << 
		name << print(range) << endl;
	    return false;
	}
    }
    catch (std::logic_error except) {
	_err << "LOGIC ERROR:\n";
	_err << except.what() << '\n';
	_err << "Please send a bug report to " << PACKAGE_BUGREPORT << endl;
	clearModel();
	return false;
    }
    catch (std::runtime_error except) {
	_err << "RUNTIME ERROR:\n";
	_err << except.what() << endl;
	clearModel();
	return false;
    }
    return true;
}

bool Console::clearMonitor(string const &name, Range const &range,
			   string const &type)
{
  if (!_model) {
    _err << "Can't clear monitor. No model!" << endl;    
    return false;
  }

  try {
      bool ok = _model->deleteMonitor(name, range, type);      
      if (!ok) {
	  _err << "Failed to clear " << type << " monitor for node " << 
	      name << print(range) << endl;
	  return false;
      }
  }
  catch (std::logic_error except) {
    _err << "LOGIC ERROR:" << endl;
    _err << except.what() << endl;
    _err << "Please send a bug report to " << PACKAGE_BUGREPORT << endl;
    clearModel();
    return false;
  }
  catch (std::runtime_error except) {
    _err << "RUNTIME ERROR:" << endl;
    _err << except.what() << endl;
    clearModel();
    return false;
  }
  return true;
}

bool Console::setDefaultMonitors(string const &type, unsigned int thin)
{
    if (!_model) {
	_err << "Can't set monitors. No model!" << endl;    
	return false;
    }

    try {
	bool ok = _model->setDefaultMonitors(type, thin);
	if (!ok) {
	    _err << "Failed to set default monitors of type " << type << endl;
	    return false;
	}
    }
    catch (std::logic_error except) {
	_err << "LOGIC ERROR:\n";
	_err << except.what() << '\n';
	_err << "Please send a bug report to " << PACKAGE_BUGREPORT << endl;
	clearModel();
	return false;
    }
    catch (std::runtime_error except) {
	_err << "RUNTIME ERROR:\n";
	_err << except.what() << endl;
	clearModel();
	return false;
    }
    return true;
}

void Console::clearModel()
{
    /*
    if (_pvariables) {
	for (unsigned int i = 0; i < _pvariables->size(); ++i) {
	    delete (*_pvariables)[i];
	}
	delete _pvariables;
	_pvariables = 0;
    }
    delete _pdata; _pdata = 0;
    delete _prelations; _prelations = 0;
    */
    _out << "Deleting model" << endl;
    delete _model; 
    _model = 0;
}

bool Console::dumpState(map<string,SArray> &data_table,
			string &rng_name,
			DumpType type, unsigned int chain)
{
  if (!_model) {
    _err << "No model" << endl;
    return false;
  }
  if (_model->symtab().size() == 0) {
    _err << "Symbol table is empty" << endl;
    return false;
  }
  if (chain == 0 || chain > nchain()) {
    _err << "Invalid chain number" << endl;
    return false;
  }
  bool (*selection)(Node const *) = 0;
  switch (type) {
  case DUMP_PARAMETERS:
    selection = isParameter;
    break;
  case DUMP_DATA:
    selection = isData;
    break;
  case DUMP_ALL:
    selection = alwaysTrue;
    break;
  }

  try {
    _model->symtab().readValues(data_table, chain - 1, selection);
    
    if (type == DUMP_PARAMETERS || type == DUMP_ALL) {
      
      vector<int> rngstate;
      if (_model->rng(chain - 1)) {
	_model->rng(chain - 1)->getState(rngstate);
	
	vector<unsigned int> dimrng(1,rngstate.size());
	SArray rngsarray(dimrng);
	rngsarray.setDiscreteValued(true);
	for (unsigned i = 0; i < rngstate.size(); ++i) {
	  rngsarray.setValue(rngstate[i], i);
	}
	
	data_table.insert(pair<string, SArray>(".RNG.state",rngsarray));
	rng_name = _model->rng(chain - 1)->name();
      }
    }
  }
  catch (NodeError except) {
    _err << "Error in node " <<
      _model->symtab().getName(except.node) << "\n";
    _err << except.what() << endl;
    return false;
  }
  catch (std::runtime_error except) {
    _err << "RUNTIME ERROR:\n";
    _err << except.what() << endl;
    return false;
  }
  catch (std::logic_error except) {
    _err << "LOGIC ERROR:\n";
    _err << except.what() << '\n';
    _err << "Please send a bug report to " << PACKAGE_BUGREPORT << endl;
    return false;
  }
  
  return true;
}


bool Console::dumpMonitors(map<string,SArray> &data_table,
			   string const &name, unsigned int chain)
{
    if(chain == 0 || chain > nchain()) {
	_err << "Invalid chain number" << endl;
	return false;
    }
    chain -= 1;

    try {
	list<Monitor*> const &monitors = _model->monitors();
	list<Monitor*>::const_iterator p;
	for (p = monitors.begin(); p != monitors.end(); ++p) {
	    Monitor const *monitor = *p;
	    if (monitor->niter() > 0 && monitor->name() == name) {
		Node const *node = monitor->node();
		string name = _model->symtab().getName(node);
		
		/*
		//The new SArray has the same dimensions as the
		//monitored node, plus an extra one for the 
		//iterations. We put the extra dimension first.
		unsigned int niter = monitor->niter();
		unsigned int ndim = node->dim().size();
		vector<unsigned int> dim(ndim + 1);
		dim[0] = niter;
		unsigned int length = 1;
		for (unsigned int i = 1; i <= ndim; ++i) {
		    dim[i] = node->dim()[i-1];
		    length *= dim[i];
		}
		*/

		//FIXME: Will need to call aperm in rjags interface
		//as extra dimension for TraceMonitors now goes LAST.

		vector<unsigned int> dim = monitor->dim();
		unsigned int length = product(dim);

		/*
		//Create a new SArray and insert it into the table
		SArray ans(dim);
		double *values = new double[length * niter];
		vector<double> const &monitor_values = monitor->value(chain);
		for (unsigned int i = 0; i < length; ++i) {
		    for (unsigned int j = 0; j < niter; ++j) {
			values[niter * i + j] = monitor_values[length * j + i];
		    }
		}
		ans.setValue(values, length*niter);
		delete [] values;
		*/

		//Create a new SArray and insert it into the table
		SArray ans(dim);
		double *values = new double[length];
		vector<double> const &monitor_values = monitor->value(chain);
		for (unsigned int i = 0; i < length; ++i) {
		    values[i] = monitor_values[i];
		}
		ans.setValue(values, length);
		delete [] values;

		data_table.insert(pair<string,SArray>(name, ans));
	    }
	}
    }
    catch (NodeError except) {
	_err << "Error in node " <<
	    _model->symtab().getName(except.node) << "\n";
	_err << except.what() << endl;
	return false;
    }
    catch (std::runtime_error except) {
	_err << "RUNTIME ERROR:\n";
	_err << except.what() << endl;
	return false;
    }
    catch (std::logic_error except) {
	_err << "LOGIC ERROR:\n";
	_err << except.what() << '\n';
	_err << "Please send a bug report to " << PACKAGE_BUGREPORT << endl;
	return false;
    }

    return true;
}

bool Console::coda(ofstream &index, vector<ofstream*> const &output)
{
    if (!_model) {
	_err << "Can't dump CODA output. No model!" << endl;
	return false;
    }

    try {
        string warn;
	_model->coda(index, output, warn);
        if (!warn.empty()) {
            _err << "WARNING:\n" << warn;
        }
    }
    catch (NodeError except) {
	_err << "Error in node " <<
	    _model->symtab().getName(except.node) << "\n";
	_err << except.what() << endl;
	return false;
    }
    catch (std::runtime_error except) {
	_err << "RUNTIME ERROR:\n";
	_err << except.what() << endl;
	return false;
    }
    catch (std::logic_error except) {
	_err << "LOGIC ERROR:\n";
	_err << except.what() << '\n';
	_err << "Please send a bug report to " << PACKAGE_BUGREPORT << endl;            return false;
    }
    return true;
}

bool Console::coda(vector<pair<string, Range> > const &nodes,
		   ofstream &index, vector<ofstream*> const &output)
{
    if (!_model) {
	_err << "Can't dump CODA output. No model!" << endl;
	return false;
    }

    /*
    NodeArray *array = _model->symtab().getVariable(name);
    if (!array) {
	_err << name << " not found" << endl;
	return false;
    }

    Node *node = 0;
    if (isNULL(range)) {
	node = array->getSubset(array->range(), _model->graph());
    }
    else if (array->range().contains(range)) {
	node = array->getSubset(range, _model->graph());
    }
    else {
	_err << "Requested invalid subset of node " << name << endl;
	return false;
    }
    if (!node) {
	_err << "Node is not being monitored" << endl;
	return false;
    }
    */

    try {
        string warn;
	_model->coda(nodes, index, output, warn);
        if (!warn.empty()) {
            _err << "WARNINGS:\n" << warn;
        }
    }
    catch (NodeError except) {
	_err << "Error in node " <<
	    _model->symtab().getName(except.node) << "\n";
	_err << except.what() << endl;
	return false;
    }
    catch (std::runtime_error except) {
	_err << "RUNTIME ERROR:\n";
	_err << except.what() << endl;
	return false;
    }
    catch (std::logic_error except) {
	_err << "LOGIC ERROR:\n";
	_err << except.what() << '\n';
	_err << "Please send a bug report to " << PACKAGE_BUGREPORT << endl;            return false;
    }
    return true;
}

BUGSModel const *Console::model()
{
  return _model;
}

/*
void jags_set_seed(unsigned int seed)
{
  // Given a single seed, calculates seeds for the Marsaglia
  // Multicarry random number generator
  unsigned int mmseeds[2];
  for(int j = 0; j < 2; j++) {
    seed = (69069 * seed + 1);
    mmseeds[j] = seed;
  }
  // Fix up zero seeds 
  if (mmseeds[0] == 0)
    mmseeds[0] = 1;
  if (mmseeds[1] == 0)
    mmseeds[1] = 1;

  set_seed(mmseeds[0], mmseeds[1]);
}
*/

unsigned int Console::nchain() const
{
  if (_model == 0) {
    return 0;
  }
  else {
    return _model->nchain();
  }
}

bool Console::adaptOff(bool &status) 
{
  if (_model == 0) {
    _err << "Can't update. No model!" << endl;
    return false;
  }
  if (!_model->isInitialized()) {
    _err << "Model not initialized" << endl;
    return false;
  }

  try {
      status =  _model->adaptOff();
  }
  catch (NodeError except) {
    _err << "Error in node " << _model->symtab().getName(except.node) << '\n';
    _err << except.what() << endl;
    return false;
  }
  catch (std::runtime_error except) {
    _err << "RUNTIME ERROR:\n";
    _err << except.what() << endl;
    return false;
  }
  catch (std::logic_error except) {
    _err << "LOGIC ERROR:\n";
    _err << except.what() << '\n';
    _err << "Please send a bug report to " << PACKAGE_BUGREPORT << endl;
    return false;
  }
  return true;
}

bool Console::isAdapting() const
{
    return _model ? _model->isAdapting() : false;
}

/*
bool Console::setRNGseed(SArray const &seed, unsigned int chain)
{
  if (_model == 0) {
    _err << "Can't set RNG seed. No model!" << endl;    
    return false;
  }
  if (seed.length() != 1) {
    _err << "Seed must be a single integer\n";
    return false;
  }
  if (chain == 0 || chain > _model->nchain()) {
    _err << "Invalid chain number\n";
    return false;
  }

  int iseed = static_cast<int>(*seed.value());
  _model->rng(chain-1)->init(iseed);
  return true;
}

bool Console::setRNGstate(SArray const &state, unsigned int chain)
{
  if (_model == 0) {
    _err << "Can't set RNG state. No model!" << endl;    
    return false;
  }
  vector<int>(istate);
  double const *value = state.value();
  for (unsigned int i = 0; i < state.length(); ++i) {
    istate.push_back(static_cast<int>(value[i]));
  }

  if (_model->rng(chain-1)->setState(istate) == false) {
    _err << "Invalid RNG state\n";
    return false;
  }
  return true;
}
*/
