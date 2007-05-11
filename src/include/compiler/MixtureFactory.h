#ifndef MIXTURE_FACTORY_H_
#define MIXTURE_FACTORY_H_

#include <graph/Graph.h>
#include <graph/Node.h>

#include <vector>
#include <map>
#include <cfloat>

class NodeArray;
class MixtureNode;

typedef std::pair<std::vector <Node const*>, std::vector<std::pair<std::vector<int> , Node const *> > > 
MixPair;

bool compMixPair(MixPair const &, MixPair const &);

/**
 * @short STL function object for the map class using MixPair as a key
 */
struct ltmixpair
{
  bool operator()(MixPair const &arg1, MixPair const &arg2) const
  {
    return compMixPair(arg1, arg2);
  }
};

/**
 * @short Factory for MixtureNode objects
 * 
 * The purpose of a MixtureFactory is to avoid unnecessary duplication
 * of mixture nodes by having a container class and factory object
 * that will create and/or lookup mixture nodes.
 */
class MixtureFactory  
{ 
  std::map<MixPair, MixtureNode*, ltmixpair> _mixmap;
public:
  /**
   * Get a mixture node.  The results are cached, so if a request is
   * repeated, the same node will be returned.
   *
   * @param index Vector of discrete-valued Nodes that index the
   * possible parameters 
   *
   * @param parameters Vector of pairs. The first element of the pair
   * shows a possible value of the index vector and the second element
   * shows the corresponding parent from which the mixture node copies
   * its value.
   *
   * @param graph Graph to which newly allocated mixturenodes are added.
   *
   */
  MixtureNode *getMixtureNode(std::vector<Node const *> const &index,
			      std::vector<std::pair<std::vector<int> , Node const *> > 
			      const &parameters,
                              Graph &graph);
};

#endif /* MIXTURE_FACTORY_H_ */
