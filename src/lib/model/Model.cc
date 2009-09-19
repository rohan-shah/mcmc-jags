#include <config.h>
#include <model/Model.h>
#include <model/MonitorFactory.h>
#include <model/Monitor.h>
#include <sampler/Sampler.h>
#include <sampler/SamplerFactory.h>
#include <rng/RNGFactory.h>
#include <rng/RNG.h>
#include <graph/GraphMarks.h>
#include <graph/StochasticNode.h>
#include <graph/DeterministicNode.h>
#include <graph/NodeError.h>
#include <graph/Node.h>

#include <fstream>
#include <sstream>
#include <set>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <functional>
#include <map>

using std::map;
using std::pair;
using std::binary_function;
using std::sort;
using std::vector;
using std::list;
using std::set;
using std::ofstream;
using std::logic_error;
using std::runtime_error;
using std::string;
using std::ostringstream;
using std::stable_sort;
using std::copy;
using std::min;
using std::max;

Model::Model(unsigned int nchain)
    : _samplers(0), _nchain(nchain), _rng(nchain, 0), _iteration(0),
      _is_initialized(false), _adapt(false), _data_gen(false)
{
}

Model::~Model()
{
    while(!_samplers.empty()) {
	Sampler *sampler0 = _samplers.back();
	delete sampler0;
	_samplers.pop_back();
    }
    for (list<Monitor*>::const_iterator p = _default_monitors.begin();
	 p != _default_monitors.end(); ++p)
    {
	delete *p;
    }
}

Graph &Model::graph() 
{
    return _graph;
}

bool Model::isInitialized()
{
    return _is_initialized;
}

void Model::chooseRNGs()
{
    /* Assign default RNG objects for any chain that does not
       currently have one */

    //Count number of unassigned RNGs
    unsigned int n  = 0;
    for (unsigned int i = 0; i < _nchain; ++i) {
	if (_rng[i] == 0)
	    ++n;
    }

    vector<RNG*> new_rngs;
    for (list<RNGFactory*>::const_iterator p = rngFactories().begin();
	 p != rngFactories().end(); ++p) 
    {
	vector<RNG*> rngs = (*p)->makeRNGs(n);
        if (rngs.size() > n) {
            throw logic_error("Too many rngs produced by RNG factory");
        }
        else {
            n -= rngs.size();
        }
	for (unsigned int j = 0; j < rngs.size(); ++j) {
	    new_rngs.push_back(rngs[j]);
	}
	if (n == 0)
	    break;
    }
    
    if (n > 0) {
	throw runtime_error("Cannot generate sufficient RNGs");
    }
    else {
	unsigned int j = 0;
	for (unsigned int i = 0; i < _nchain; ++i) {
	    if (_rng[i] == 0) {
		_rng[i] = new_rngs[j++];
	    }
	}
    }
}

void Model::initialize(bool datagen)
{
    if (_is_initialized)
	throw logic_error("Model already initialized");

    if (!_graph.isClosed())
	throw runtime_error("Graph not closed");

    // Choose random number generators
    chooseRNGs();

    //Initialize nodes
    initializeNodes();

    // Choose Samplers
    chooseSamplers();
    
    if (datagen) {
	Graph egraph;
	for (set<Node *>::const_iterator p = _extra_nodes.begin(); 
	     p != _extra_nodes.end(); ++p)
	{
	    egraph.add(*p);
	}
	_sampled_extra.clear();
	egraph.getSortedNodes(_sampled_extra);
	_data_gen = true;
    }

    // Switch to adaptive mode if we find an adaptive sampler
    for (unsigned int i = 0; i < _samplers.size(); ++i) {
	if (_samplers[i]->isAdaptive()) {
	    _adapt = true;
	    break;
	}
    }
    
    _is_initialized = true;
}

void Model::initializeNodes()
{
    //Get nodes in forward-sampling order
    vector<Node*> sorted_nodes;
    _graph.getSortedNodes(sorted_nodes);

    vector<Node*>::const_iterator i;
    for (i = sorted_nodes.begin(); i != sorted_nodes.end(); ++i) {
	Node *node = *i;
	for (unsigned int n = 0; n < _nchain; ++n) {
	    if (!node->checkParentValues(n)) {
		throw NodeError(node, "Invalid parent values");
	    }
	    if (!node->initialize(_rng[n], n)) {
		throw NodeError(node, "Initialization failure");
	    } 
	}
    }
}

struct less_sampler {  
    /* 
       Comparison operator for Samplers which sorts them according to
       the ordering defined by the supplied sampler_map.
    */
    map<Sampler const*, vector<unsigned int> > const & _sampler_map;

    less_sampler(map<Sampler const*, vector<unsigned int> > const &sampler_map) 
	: _sampler_map(sampler_map) {};

    bool operator()(Sampler const *x, Sampler const *y) const {
	map<Sampler const*, vector<unsigned int> >::const_iterator i 
	    = _sampler_map.find(x);
	map<Sampler const*, vector<unsigned int> >::const_iterator j 
	    = _sampler_map.find(y);

	if (i == _sampler_map.end() || j == _sampler_map.end()) {
	    throw logic_error("Invalid sampler map");
	}
	return i->second < j->second;
    };
};

static bool anyChildInSet(Sampler const *sampler, 
			  ConstStochasticNodeSet const &sset)
{
    vector<StochasticNode const*> const &schildren 
	= sampler->stochasticChildren();
    for (unsigned int i = 0; i < schildren.size(); ++i) {
	if (sset.find(schildren[i]) != sset.end())
	    return true;
    }
    return false;
}

static void 
samplerLimits(Sampler const *sampler, unsigned int &lower, unsigned int &upper)
{
    //Gather the sampled nodes and deterministic children into a test set
    set<Node const *> testset;
    testset.insert(sampler->nodes().begin(), sampler->nodes().end());
    testset.insert(sampler->deterministicChildren().begin(), 
		   sampler->deterministicChildren().end());
    
    vector<StochasticNode const *> const &sch = sampler->stochasticChildren();

    lower = upper = 0; //in case sch is empty
    
    for (unsigned int i = 0; i < sch.size(); ++i) {
	
	vector<Node const *> const &parents = sch[i]->parents();
	unsigned int lp = parents.size();
	unsigned int up = 0;

	for (unsigned int j = 0; j < parents.size(); ++j) {
	    if (testset.find(parents[j]) != testset.end()) {
		lp = min(j, lp);
		up = max(j, up);
	    }
	}
	if (lp > up) {
	    throw logic_error("Invalid sampler");
	}

	if (i == 0) {
	    lower = lp;
	    upper = up;
	}
	else {
	    lower = min(lower, lp);
	    upper = max(upper, up);
	}
    }
}

void Model::chooseSamplers()
{
    /*
     * Selects samplers. Samplers are selected by traversing the list
     * of SamplerFactories in order. If there are any informative
     * stochastic nodes left without samplers after all factories have
     * been tried, then a runtime error is thrown
     *
     * @see Model#samplerFactories
     */

    GraphMarks marks(_graph);
    Graph sample_graph;
    set<Sampler*> samplerSet;

    // Add observed stochastic nodes to the sample graph and mark
    // the informative nodes
    StochasticNodeSet const &stoch_nodes = _graph.stochasticNodes();
    StochasticNodeSet::const_iterator p;
    for (p = stoch_nodes.begin(); p != stoch_nodes.end(); ++p) {
	if ((*p)->isObserved()) {
	    sample_graph.add(*p);
	    marks.markAncestors(*p, 1);
	}
    }
    for (p = stoch_nodes.begin(); p != stoch_nodes.end(); ++p) {
	if ((*p)->isObserved()) {
	    marks.mark(*p, 2);
	}
    }

    //Triage on marked nodes. We do this twice: once for stochastic
    //nodes and once for all nodes.

    StochasticNodeSet sset;
    for(p = stoch_nodes.begin(); p != stoch_nodes.end(); ++p) {
	switch(marks.mark(*p)) {
	case 0:
	    _extra_nodes.insert(*p);
	    break;
	case 1:
	    sset.insert(*p); 
	    //falling through!
	case 2:
	    sample_graph.add(*p);
	    break;
	default:
	    throw logic_error("Invalid mark");
	}
    }

    set<Node*> const &nodes = _graph.nodes();
    set<Node*>::const_iterator j;
    for (j = nodes.begin(); j != nodes.end(); ++j) {
	switch(marks.mark(*j)) {
	case 0:
	    _extra_nodes.insert(*j);
	    break;
	case 1: case 2:
	    sample_graph.add(*j);
	    break;
	}
    }

    // Traverse the list of samplers, selecting nodes that can be sampled
    list<SamplerFactory const *> const &sfactories = samplerFactories();
    for (list<SamplerFactory const *>::const_iterator p = sfactories.begin();
	 p != sfactories.end(); ++p)
    {
	Sampler *sampler = (*p)->makeSampler(sset, sample_graph);
	while (sampler) {
	    vector<StochasticNode*> const &nodes = sampler->nodes();
	    for (unsigned int i = 0; i < nodes.size(); ++i) {
		sset.erase(nodes[i]);
	    }
	    samplerSet.insert(sampler);
	    sampler = (*p)->makeSampler(sset, sample_graph);
	}
    }
  
    // Make sure we found a sampler for all the nodes
    if (!sset.empty()) {
      
	throw NodeError(*sset.begin(),
			"Unable to find appropriate sampler");
    }
  
    // Now sort the samplers in order

    // Collect nodes to be sampled in another set. This one contains
    // constant pointers.

    ConstStochasticNodeSet sset2;
    for(p = stoch_nodes.begin(); p != stoch_nodes.end(); ++p) {
	if (marks.mark(*p) == 1) {
	    sset2.insert(*p);
	}
    }


    // Samplers are sorted in reverse sampling order: i.e. samplers
    // that are closer to the data are updated before samplers that
    // only affect higher-order parameters

    _samplers.reserve(samplerSet.size());

    while (!samplerSet.empty()) {

	// Vector of samplers that may be pushed onto the stack
	vector<Sampler*> vsamplers;

	for(set<Sampler*>::iterator i = samplerSet.begin(); 
	    i != samplerSet.end(); ++i) 
	{
	    if (!anyChildInSet(*i, sset2)) {
		vsamplers.push_back(*i);
	    }
	}
	if (vsamplers.empty()) {
	    throw logic_error("Loop check failed in Model::chooseSamplers");
	}

	/* Order vsamplers so that a sampler that affects lower order
	   parameters of a stochastic node (e.g. a location parameter)
	   acts before a sampler that affects higher order parameters
	   (e.g. a precision parameter)
	*/

	map<Sampler const *, vector<unsigned int> > sampler_map;
	vector<unsigned int> limits(2);

	for (unsigned int j = 0; j < vsamplers.size(); ++j) {
	    samplerLimits(vsamplers[j], limits[0], limits[1]);
	    sampler_map.insert(pair<Sampler const*, vector<unsigned int> >(vsamplers[j], limits));
	}
	stable_sort(vsamplers.begin(), vsamplers.end(), 
		    less_sampler(sampler_map));

	for (unsigned int j = 0; j < vsamplers.size(); ++j) {
	    _samplers.push_back(vsamplers[j]);
	    samplerSet.erase(vsamplers[j]);
	    for (unsigned int k = 0; k < vsamplers[j]->nodes().size(); ++k) {
		sset2.erase(vsamplers[j]->nodes()[k]);
	    }
	}
    }
}

void Model::update(unsigned int niter)
{
    if (!_is_initialized) {
	throw logic_error("Attempt to update uninitialized model");
    }

    list<Monitor*>::const_iterator p;
    for (p = _monitors.begin(); p != _monitors.end(); ++p) {
	(*p)->reserve(niter);
    }

    for (unsigned int iter = 0; iter < niter; ++iter) {    
	
	for (vector<Sampler*>::iterator i = _samplers.begin(); 
	     i != _samplers.end(); ++i) 
	{
	    (*i)->update(_rng);
	}

	for (unsigned int n = 0; n < _nchain; ++n) {
	    for (vector<Node*>::const_iterator k = _sampled_extra.begin();
		 k != _sampled_extra.end(); ++k)
	    {
                if (!(*k)->checkParentValues(n)) {
                     throw NodeError(*k, "Invalid parent values");
                }
		(*k)->randomSample(_rng[n], n);
	    }
	}
	_iteration++;

	for (list<Monitor*>::iterator k = _monitors.begin(); 
	     k != _monitors.end(); k++) 
	{
	    (*k)->update(_iteration);
	}
    }
}

unsigned int Model::iteration() const
{
  return _iteration;
}

bool Model::adaptOff() 
{
    bool status = true;

    for (vector<Sampler*>::iterator p = _samplers.begin(); 
	 p != _samplers.end(); ++p)
    {
	if (!(*p)->adaptOff())
	    status = false;
    }

    _adapt = false;
    return status;
}

bool Model::isAdapting() const
{
  return _adapt;
}


void Model::setSampledExtra()
{
    /* If a mode is not a data generating model, uninformative nodes
       do not need to be updated, unless they have a descendant that 
       is being monitored. This function finds those nodes and adds
       them to the vector _sampled_extra.
    */
       
    if (_data_gen) {
	// In a data generating model, all uninformative nodes are
	// sampled, so nothing to be done
	return;
    }

    // Recalculate the vector of uninformative nodes that need sampling
	
    //Insert extra nodes into a new graph
    Graph egraph;
    for (set<Node *>::const_iterator p = _extra_nodes.begin(); 
	 p != _extra_nodes.end(); ++p)
    {
	egraph.add(*p);
    }
    //Mark the ancestors of all monitored nodes in this graph
    GraphMarks emarks(egraph);
    for (list<Monitor*>::const_iterator p = _monitors.begin();
	 p != _monitors.end(); ++p)
    {
	Node const *node = (*p)->node();
	if (egraph.contains(node)) {
	    emarks.mark(node, 1);
	    emarks.markAncestors(node, 1);
	}
    }
    //Remove unmarked nodes from graph
    for (set<Node *>::const_iterator p = _extra_nodes.begin(); 
	 p != _extra_nodes.end(); ++p)
    {
	if (emarks.mark(*p) == 0)
	    egraph.remove(*p);
    }
    //Replace vector of sampled extra nodes
    _sampled_extra.clear();
    egraph.getSortedNodes(_sampled_extra);
}



void Model::addMonitor(Monitor *monitor)
{
    if (_adapt) {
	throw logic_error("Cannot add monitor to adapting model");
    }
    
    _monitors.push_back(monitor);
    setSampledExtra();
}

void Model::removeMonitor(Monitor *monitor)
{
    _monitors.remove(monitor);
    setSampledExtra();
}

void Model::addExtraNode(Node *node)
{
    if (!_is_initialized) {
	throw logic_error("Attempt to add extra node to uninitialized model");
    }
    if (node->isObserved()) {
	throw logic_error("Cannot add observed node to initialized model");
    }
    if (!node->stochasticChildren()->empty() || !node->deterministicChildren()->empty()) {
	throw logic_error("Cannot add extra node with children");
    }

    if (_extra_nodes.count(node)) {
	throw logic_error("Extra node already in model");
    }
    
    for (vector<Node const *>::const_iterator p = node->parents().begin(); 
	 p != node->parents().end(); ++p)
    {
	if (!_graph.contains(*p)) {
	throw logic_error("Extra node has parents not in model");
	}
    }

    if (!_graph.contains(node)) {
	_graph.add(node);
    }

    _extra_nodes.insert(node);
    if (_data_gen) {
	//Extra nodes are automatically sampled
	_sampled_extra.push_back(node);
    }
}

list<SamplerFactory const *> &Model::samplerFactories()
{
  static list<SamplerFactory const *> _samplers;
  return _samplers;
}

list<RNGFactory *> &Model::rngFactories()
{
  static list<RNGFactory *> _rngfac;
  return _rngfac;
}

list<MonitorFactory *> &Model::monitorFactories()
{
    static list<MonitorFactory *> _monitorfac;
    return _monitorfac;
}

unsigned int Model::nchain() const
{
  return _nchain;
}

RNG *Model::rng(unsigned int chain) const
{
  return _rng[chain];
}

bool Model::setRNG(string const &name, unsigned int chain)
{
  /* Assign default RNG objects for any chain that does not
     currently have one */

  if (chain >= _nchain)
     throw logic_error("Invalid chain number in Model::setRNG");

  list<RNGFactory*>::const_iterator p;
  for (p = rngFactories().begin(); p != rngFactories().end(); ++p) {
    RNG *rng = (*p)->makeRNG(name);
    if (rng) {
      _rng[chain] = rng;
      return true;
    }
  }

  return false;
}

bool Model::setRNG(RNG *rng, unsigned int chain)
{
  if (chain >= _nchain)
     throw logic_error("Invalid chain number in Model::setRNG");

  _rng[chain] = rng;
  return true;
}

list<Monitor*> const &Model::monitors() const
{
  return _monitors;
}

bool Model::setDefaultMonitors(string const &type, unsigned int thin)
{
    list<MonitorFactory*> const &faclist = monitorFactories();

    for(list<MonitorFactory*>::const_iterator j = faclist.begin();
	j != faclist.end(); ++j)
    {
	vector <Node const *> default_nodes = (*j)->defaultNodes(this, type);
	if (!default_nodes.empty()) {
	    unsigned int start = iteration() + 1;
	    for (unsigned int i = 0; i < default_nodes.size(); ++i) {
		Monitor *monitor = (*j)->getMonitor(default_nodes[i], this,
						    start, thin, type);
		if (!monitor) {
		    throw logic_error("Invalid default monitor");
		}
		addMonitor(monitor);
		/* Model takes ownership of default monitors */
		_default_monitors.push_back(monitor);
	    }
	    return true;
	}
    }
    return false;
}

void Model::clearDefaultMonitors(string const &type)
{
    list<Monitor*> dmonitors = _default_monitors;
    for (list<Monitor*>::const_iterator p = dmonitors.begin();
	 p != dmonitors.end(); ++p) 
    {
	Monitor *monitor = *p;
	if (monitor->type() == type) {
	    _default_monitors.remove(monitor);
	    _monitors.remove(monitor);
	    delete monitor;
	}
    }
    setSampledExtra();
}

