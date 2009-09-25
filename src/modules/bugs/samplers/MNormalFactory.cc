#include <config.h>

#include "MNormal.h"
#include "MNormalFactory.h"
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <sampler/ParallelSampler.h>
#include <sampler/Updater.h>

#include <string>
#include <vector>

using std::vector;


bool 
MNormalFactory::canSample(StochasticNode * snode, Graph const &graph) const
{
    return snode->distribution()->name() == "dmnorm" && !isBounded(snode);
}

Sampler *
MNormalFactory::makeSampler(StochasticNode *snode, Graph const &graph) const
{
    unsigned int N = snode->nchain();
    vector<SampleMethod*> methods(N, 0);

    Updater *updater = new Updater(snode, graph);
    for (unsigned int ch = 0; ch < N; ++ch) {
        methods[ch] = new MNormMetropolis(updater, ch);
    }
    return new ParallelSampler(updater, methods);
}
