#include <config.h>

#include <string>

#include "ProbitFactory.h"
#include "Probit.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <function/InverseLinkFunc.h>

using std::string;

//debuggin
#include <iostream>

namespace glm {

    bool ProbitFactory::checkOutcome(StochasticNode const *snode) const
    {
	string const &dname = snode->distribution()->name();
	if (dname == "dbern") {
	    std::cout << "dbern" << std::endl;
	    return true;
	}
	else if (dname == "dbin") {
	    std::cout << "dbin" << std::cout;
	    Node const *N = snode->parents()[1];
	    if (N->length() != 1)
		return false;
	    if (!N->isObserved())
		return false;
	    if (N->value(0)[0] != 1)
		return false;
	    return true;
	}
	else {
	    std::cout << "Something else (" << dname << ")" << std::endl;
	    return false;
	}
    }
    
    bool ProbitFactory::checkLink(InverseLinkFunc const *link) const
    {
	if (link->linkName() == "probit") {
	    std::cout << "probit!" << std::endl;
	}
	else {
	    std::cout << "Not probit (" << link->linkName() << ")" << std::endl;
	}
	return link->linkName() == "probit";
    }

    GLMMethod *ProbitFactory::newMethod(Updater const *updater,
					     unsigned int chain) const
    {
	return new Probit(updater, chain);
    }

}
