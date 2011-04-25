#include <config.h>

#include "MNormal.h"
#include "MNormalFactory.h"
#include <graph/StochasticNode.h>
#include <distribution/Distribution.h>
#include <sampler/ParallelSampler.h>
#include <sampler/GraphView.h>

#include <string>
#include <vector>

using std::vector;
using std::string;

namespace bugs {

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

    GraphView *gv = new GraphView(snode, graph);
    for (unsigned int ch = 0; ch < N; ++ch) {
        methods[ch] = new MNormMetropolis(gv, ch);
    }
    return new ParallelSampler(gv, methods);
}

string MNormalFactory::name() const 
{
    return "bugs::MNormal";
}

}
