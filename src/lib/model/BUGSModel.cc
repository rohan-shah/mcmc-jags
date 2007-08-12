#include <config.h>
#include <model/BUGSModel.h>
#include <model/TraceMonitor.h>
#include <model/NodeArray.h>
#include <graph/DevianceNode.h>
#include <graph/StochasticNode.h>
#include <graph/GraphMarks.h>
#include <graph/Node.h>
#include <rng/RNG.h>

#include <list>
#include <utility>
#include <stdexcept>
#include <fstream>
#include <cmath>

//FIXME: do we still need all of these?
using std::vector;
using std::ofstream;
using std::list;
using std::pair;
using std::string;
using std::logic_error;
using std::runtime_error;
using std::set;
using std::map;

/* 
   Nodes accessible to the user in a BUGSModel are identified
   by a variable name and range of indices 
*/
typedef pair<string, Range> NodeId;

BUGSModel::BUGSModel(unsigned int nchain)
  : Model(nchain), _symtab(graph(), nchain)
{
}

SymTab &BUGSModel::symtab() 
{
  return _symtab;
}

Node *BUGSModel::getNode(string const &name, Range const &target_range,
			 string &message)
{
    NodeArray *array = _symtab.getVariable(name);
    if (!array) {
	message = string("Unknown variable ") + name;
	return 0;
    }

    Range range = target_range;
    if (isNULL(range)) {
	range = array->range();
    }
    else if (!array->range().contains(target_range)) {
	//FIXME. What if target_range has wrong dimension?
	message = string("Invalid range ") + name + print(target_range);
	return 0;
    }
    message.clear();
    unsigned int NNode = graph().size();
    Node *node = array->getSubset(range, graph());
    if (graph().size() != NNode) {
	addExtraNode(node); // Node was newly allocated
    }
    return node;
}

#include <sarray/nainf.h>
static void writeDouble(double x, std::ostream &out)
{
    if (x == JAGS_NA) {
	out << "NA";
    }
    else if (jags_isnan(x)) {
	out << "NaN";
    }
    else if (!jags_finite(x)) {
	if (x > 0) 
	    out << "Inf";
	else 
	    out << "-Inf";
    }
    else {
	out << x;
    }
}

static void CODA(map<NodeId,TraceMonitor*> const &trace_map,
		 ofstream &index,  vector<ofstream*> const &output)
{
    //FIXME: Check streams are open and set pointer to beginning.

    unsigned int lineno = 0;
    map<NodeId, TraceMonitor*>::const_iterator p = trace_map.begin();;
    for (; p != trace_map.end(); ++p) {

	TraceMonitor const *monitor = p->second;
	if (!isSynchronized(monitor)) {
	    throw logic_error(" Unsynchronized Monitor in CODA");
	}

	string const &name = p->first.first;
	Range const &uni_range = p->first.second;
	Range multi_range = uni_range;
	if (isNULL(multi_range)) {
	    multi_range = Range(monitor->node()->dim());
	}
	else if (multi_range.dim(true) != monitor->node()->dim()) {
	    throw logic_error("Range does not match Node dimension in CODA");
	}

	unsigned int nvar = monitor->node()->length();
	if (nvar != 1) {
	    // Multivariate node
	    for (unsigned int offset = 0; offset < nvar; ++offset) 
	    {
		index << name << print(multi_range.leftIndex(offset)) << " "  
		      << lineno + 1 << "  "  
		      << lineno + monitor->niter(0) << '\n';
		lineno += monitor->niter(0);
	    }
	}
	else {
	    // Univariate node 
	    index << name << print(uni_range) << "  " 
		  << lineno + 1 << "  " 
		  << lineno + monitor->niter(0) << '\n';
	    lineno += monitor->niter(0);
	}
	//Write output files
	for (unsigned int ch = 0; ch < output.size(); ++ch) {
	    ofstream &out = *output[ch];
	    vector<double> const &y = monitor->values(ch);
	    for (unsigned int offset = 0; offset < nvar; ++offset) {
		unsigned int iter = monitor->start();
		for (unsigned int k = 0; k < monitor->niter(ch); k++) {
		    out << iter << "  ";
		    writeDouble(y[k * nvar + offset], out);
		    out << '\n';
		    iter += monitor->thin();
		}
	    }
	}
    }
}

void BUGSModel::coda(vector<NodeId> const &nodes, ofstream &index, 
		     vector<ofstream*> const &output, string &warn)
{
    if (output.size() != nchain()) {
        throw logic_error("Wrong number of output streams in BUGSModel::coda");
    }

    warn.clear();

    // Create a new map with only the selected nodes in it 
    map<NodeId, TraceMonitor*> dump_nodes;
    for (unsigned int i = 0; i < nodes.size(); ++i) {
	map<NodeId, TraceMonitor*>::const_iterator p = 
	    _trace_map.find(nodes[i]);
	if (p == _trace_map.end()) {
	    string msg  = string("Node ") + p->first.first + 
                print(p->first.second) + " is not being monitored\n";
	    warn.append(msg);
	}
	else {
	    dump_nodes.insert(*p);
	}
    }

    if (dump_nodes.empty()) {
	warn.append("There are no monitored nodes\n");
    }
    
    CODA(dump_nodes, index, output);
}

void BUGSModel::coda(ofstream &index, vector<ofstream*> const &output,
                     string &warn)
{
    if (output.size() != nchain()) {
        throw logic_error("Wrong number of output streams in BUGSModel::coda");
    }
    
    warn.clear();
    if (_trace_map.empty()) {
	warn.append("There are no monitored nodes\n");
    }
 
    CODA(_trace_map, index, output);
}

void BUGSModel::addDevianceNode()
{
    NodeArray const *array = _symtab.getVariable("deviance");
    if (array)
	return; //Deviance already defined by user

    _symtab.addVariable("deviance", vector<unsigned int>(1,1));
    NodeArray *deviance = _symtab.getVariable("deviance");
    vector<Node*> nodes;
    graph().getNodes(nodes);
    std::set<StochasticNode const *> parameters;
    for (vector<Node*>::const_iterator p = nodes.begin(); p != nodes.end(); ++p)
    {
	if ((*p)->isObserved()) {
	    StochasticNode *snode = dynamic_cast<StochasticNode*>(*p);
	    if (snode)
		parameters.insert(snode);
	}
    }
    if (!parameters.empty()) {
	//Can't construct a deviance node with no parameters
	DevianceNode *dnode = new DevianceNode(parameters);
	addExtraNode(dnode);
	deviance->insert(dnode, vector<unsigned int>(1,1));
    }
}

void BUGSModel::setParameters(std::map<std::string, SArray> const &param_table,
			      unsigned int chain)
{
    set<Node*> setnodes;
    _symtab.writeValues(param_table, chain, setnodes);


    //Strip off .RNG.seed (user-supplied random seed)
    if (param_table.find(".RNG.seed") != param_table.end()) {
	if (rng(chain) == 0) {
	    throw runtime_error(".RNG.seed supplied RNG type not set");
	}
	SArray const &seed = param_table.find(".RNG.seed")->second;
	if (seed.length() != 1) {
	    throw runtime_error(".RNG.seed must be a single integer");
	}
	if (*seed.value() < 0) {
	    throw runtime_error(".RNG.seed must be non-negative");
	}
	int iseed = static_cast<int>(*seed.value());
	rng(chain)->init(iseed);
    }

    //Strip off .RNG.state (saved state from previous run)
    if (param_table.find(".RNG.state") != param_table.end()) {
	if (rng(chain) == 0) {
	    throw runtime_error(".RNG.state supplied, but RNG type not set");
	}
	SArray const &state = param_table.find(".RNG.state")->second;
	vector<int>(istate);
	double const *value = state.value();
	for (unsigned int i = 0; i < state.length(); ++i) {
	    istate.push_back(static_cast<int>(value[i]));
	}
	if (rng(chain)->setState(istate) == false) {
	    throw runtime_error("Invalid .RNG.state");
	}
    }
}


bool BUGSModel::setMonitor(string const &name, Range const &range,
	        unsigned int thin)
{
    pair<string, Range> nodeid(name, range);
    if (_trace_map.find(nodeid) != _trace_map.end()) {
	return false; //Node is already being monitored.
    }
    string msg;
    Node *node = getNode(name, range, msg);
    if (node) { 
	TraceMonitor *monitor = new TraceMonitor(node, iteration() + 1, thin);
	_trace_monitors.push_back(monitor);
	_trace_map[nodeid] = monitor;
	addMonitor(monitor);
	return true;
    }
    else {
	return false;
    }
}

bool BUGSModel::clearMonitor(string const &name, Range const &range)
{
    pair<string, Range> nodeid(name, range);
    map<pair<string, Range>, TraceMonitor*>::iterator p =
	_trace_map.find(nodeid);
    
    if (p == _trace_map.end()) {
	return false;
    }
    else {
	removeMonitor(p->second);
	_trace_map.erase(p);
	_trace_monitors.remove(p->second);
	return true;
    }
}

list<TraceMonitor const *> const &BUGSModel::traceMonitors() const
{
    return _trace_monitors;
}
