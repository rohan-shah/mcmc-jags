#ifndef SLICE_FACTORY_H_
#define SLICE_FACTORY_H_

#include <sampler/SingletonFactory.h>
class StochasticNode;
class Graph;

namespace base {

/**
 * @short Factory object for slice samplers
 */
    class SliceFactory : public SingletonFactory
    {
	std::string _name;
    public:
	SliceFactory();
	bool canSample(StochasticNode *snode, Graph const &graph) const;
	Sampler *makeSampler(StochasticNode *snode, Graph const &graph) const;
	std::string const &name() const;
    };

}

#endif /* SLICE_FACTORY_H_ */
