#include <config.h>

#include "Dirichlet.h"
#include "DirichletFactory.h"
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <sampler/DensitySampler.h>
#include <sampler/Updater.h>

#include <string>
#include <vector>

using std::vector;


bool 
DirichletFactory::canSample(StochasticNode * snode, Graph const &graph) const
{
    return snode->distribution()->name() == "ddirch";
}

Sampler *
DirichletFactory::makeSampler(StochasticNode *snode, Graph const &graph) const
{
    unsigned int nchain = snode->nchain();
    vector<DensityMethod*> methods(nchain, 0);
    vector<StochasticNode*> nodes(1, snode);
    Updater *updater = new Updater(snode, graph);
    for (unsigned int ch = 0; ch < nchain; ++ch) {
        methods[ch] = new DirchMetropolis(updater, ch);
    }
    return new DensitySampler(updater, methods);
}
