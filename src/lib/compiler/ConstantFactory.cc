#include <config.h>
#include <compiler/ConstantFactory.h>
#include <graph/ConstantNode.h>
#include <compiler/NodeFactory.h>

#include <sstream>
#include <cmath>
#include <cfloat>

using std::map;
using std::ostringstream;
using std::fabs;

ConstantFactory::ConstantFactory(unsigned int nchain)
    : _nchain(nchain)
{
}

ConstantNode * ConstantFactory::getConstantNode(double value, Graph &graph)
{
    ConstantNode *cnode = 0;

    map<const double,ConstantNode*, ltdouble>::const_iterator i 
	= _constmap.find(value);
    if (i == _constmap.end()) {
	// Create a new constant node
	long ivalue;
	/* FIXME: This is, unfortunately, a big problem casting doubles to
	   long that is used everywhere, and is currently broken for
	   negative numbers
	*/
	if (value >= 0) {
	    ivalue = static_cast<long>(value + DBL_EPSILON);
	}
	else {
	    ivalue = static_cast<long>(value - DBL_EPSILON);
	}
	if (fabs(value - ivalue) < DBL_EPSILON) {
	    // Integer value 
	    cnode = new ConstantNode(ivalue, _nchain);
	}
	else {
	    // Floating point value
	    cnode = new ConstantNode(value, _nchain);
	}

	_constmap[value] = cnode;
	graph.add(cnode);
	return cnode;
    }
    else {
	return i->second;
    }
}
