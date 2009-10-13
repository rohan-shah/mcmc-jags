#include <config.h>

#include <string>

#include "LogitFactory.h"
#include "Logit.h"

#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <function/InverseLinkFunc.h>

using std::string;
using std::vector;

namespace glm {

    LogitFactory::LogitFactory()
	: GLMFactory("Logit")
    {}

    bool LogitFactory::checkOutcome(StochasticNode const *snode) const
    {
	string const &dname = snode->distribution()->name();
	if (dname == "dbern") {
	    return true;
	}
	else if (dname == "dbin") {
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
	    return false;
	}
    }
    
    bool LogitFactory::checkLink(InverseLinkFunc const *link) const
    {
	return link->linkName() == "logit";
    }

    GLMMethod *
    LogitFactory::newMethod(Updater const *updater,
			     vector<Updater const *> const &sub_updaters,
			     unsigned int chain) const
    {
	return new Logit(updater, sub_updaters, chain);
    }

}
