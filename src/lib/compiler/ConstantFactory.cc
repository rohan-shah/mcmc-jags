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
using std::vector;

ConstantFactory::ConstantFactory(unsigned int nchain)
    : _nchain(nchain)
{
}

ConstantNode * ConstantFactory::getConstantNode(double value, Graph &graph)
{
    ConstantNode *cnode = 0;

    map<const double, ConstantNode*, ltdouble>::const_iterator i 
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

ConstantNode * ConstantFactory::getConstantNode(vector<unsigned int> const &dim,
						vector<double> const &value,
                                                Graph &graph)
{
    ConstantNode *cnode = 0;

    constpair cp(dim, value);

    map<constpair, ConstantNode*>::const_iterator i = _mv_constmap.find(cp);

    vector<double> ivalue = value;

    if (i == _mv_constmap.end()) {
	// Create a new constant node
	bool is_discrete = true;

	for (unsigned int j = 0; j < value.size(); ++j) {
	    long iv;
	    if (value[j] >= 0) {
		iv = static_cast<long>(value[j] + DBL_EPSILON);
	    }
	    else {
		iv = static_cast<long>(value[j] - DBL_EPSILON);
	    }
	    if (fabs(value[j] - iv) < DBL_EPSILON) {
                ivalue[j] = iv;
            }
            else {
		is_discrete = false;
		break;
	    }
	}
	
	if (is_discrete) {
	    cnode = new ConstantNode(dim, ivalue, _nchain);
	}
	else {
	    cnode = new ConstantNode(dim, value, _nchain);
	}
	
	_mv_constmap[cp] = cnode;
	graph.add(cnode);
	return cnode;
    }
    else {
	return i->second;
    }

}
