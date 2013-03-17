#include <config.h>
#include <compiler/ConstantFactory.h>
#include <model/Model.h>
#include <graph/ConstantParameterNode.h>
#include <compiler/NodeFactory.h>
#include <util/integer.h>

#include <sstream>
#include <cmath>
#include <cfloat>

using std::map;
using std::ostringstream;
using std::fabs;
using std::vector;
using std::sqrt;

namespace jags {

ConstantFactory::ConstantFactory(unsigned int nchain)
    : _nchain(nchain)
{
}

ConstantNode * ConstantFactory::getConstantNode(double value, Model &model)
{
    ConstantNode *cnode = 0;

    map<double, ConstantNode*, ltdouble>::const_iterator i 
	= _constmap.find(value);
    if (i == _constmap.end()) {
	if (checkInteger(value)) {
	    cnode = new ConstantParameterNode(asInteger(value), _nchain);
	}
	else {
	    cnode = new ConstantParameterNode(value, _nchain);
	}
	
	_constmap[value] = cnode;
	model.addNode(cnode);
	return cnode;
    }
    else {
	return i->second;
    }
}

ConstantNode * ConstantFactory::getConstantNode(vector<unsigned int> const &dim,
						vector<double> const &value,
                                                Model &model)
{
    ConstantNode *cnode = 0;

    constpair cp(dim, value);

    map<constpair, ConstantNode*>::const_iterator i = _mv_constmap.find(cp);

    vector<double> ivalue = value;

    if (i == _mv_constmap.end()) {
	// Create a new constant node
	bool is_discrete = true;

	for (unsigned int j = 0; j < value.size(); ++j) {
	    is_discrete = checkInteger(value[j]);
	    if (!is_discrete) {
		break;
	    }
	    ivalue[j] = asInteger(value[j]);
	}
	
	if (is_discrete) {
	    cnode = new ConstantParameterNode(dim, ivalue, _nchain);
	}
	else {
	    cnode = new ConstantParameterNode(dim, value, _nchain);
	}
	
	_mv_constmap[cp] = cnode;
	model.addNode(cnode);
	return cnode;
    }
    else {
	return i->second;
    }

}

} //namespace jags
