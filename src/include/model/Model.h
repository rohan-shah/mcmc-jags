#ifndef MODEL_H_
#define MODEL_H_

#include <graph/Graph.h>

#include <vector>
#include <list>

class Sampler;
class Monitor;
class SamplerFactory;
class RNG;
class RNGFactory;

/**
 * @short Current status of a chain
 */
struct ChainInfo
{
  RNG *rng;
  std::vector<Sampler*> samplers;
  unsigned int iteration;
};

/**
 * @short Graphical model 
 *
 * The purpose of the model class is to collect together all the
 * elements necessary to run an MCMC sampler on a graphical model.
 */
class Model {
  unsigned int _nchain;
  std::vector<ChainInfo> _chain_info;
  Graph _graph;
  std::set<Node*> _extra_nodes;
  std::vector<Node*> _sampled_extra;
  std::list<Monitor*> _monitors;
  bool _is_graph_checked;
  bool _is_initialized;
  bool _can_sample;
  bool _adapt;
  void initializeNodes(std::vector<Node*> const &sorted_nodes, bool random);
  void chooseRNGs();
  void chooseSamplers(std::vector<Node*> const &sorted_nodes);
public:
  /**
   * @param nchain Number of parallel chains in the model.
   */
  Model(unsigned int nchain);
  virtual ~Model();
  /**
   * Returns the Graph associated with the model. This graph contains
   * all the nodes in the model
   */
  Graph &graph();
  /**
   * Initializes the model.  Initialization takes place in three steps.
   *
   * Firstly, random number generators are assigned to any chain that
   * doesn not already have an RNG.
   * 
   * Secondly, all nodes in the graph are initialized in forward
   * sampling order. By default, this is done deterministically using
   * Node#initialize. Random initialization, using Node#randomSample,
   * may be obtained by setting the random parameter to true.
   *
   * Finally, samplers are chosen for informative nodes in the graph
   *
   * @see Node#initialize, Model#rngFactories
   */
  void initialize(bool random = false);
  /** Returns true if the model has been initialized */
  bool isInitialized();
  /**
   * Returns true if chooseSamplers has been called.
   */
  bool canSample() const;
  /** 
   * Updates the model by the given number of iterations. A
   * logic_error is thrown if the model is uninitialized.
   *
   * @param niter Number of iterations to run
   */
  void update(unsigned int niter);
  /**
   * Returns the current iteration number for the given chain. Chains
   * are enumerated starting from 0.
   */
  unsigned int iteration(unsigned int chain) const;
  /**
   * Adds a monitor to the model so that it will be updated at each
   * iteration.  This can only be done if Model#adaptOff has been
   * successfully called. Otherwise, a logic_error is thrown.
   */
  void addMonitor(Monitor *monitor);
  /**
   * Clears the monitor from the model, so that it will no longer
   * be updated. If the monitor has not previously been added to the
   * model, this function has no effect.
   */
  void removeMonitor(Monitor *monitor);
  /**
   * After the model is initialized, extra uninformative nodes may be
   * added to the graph (For example, a node may be added that
   * calculates the deviance of the model).  The model takes
   * responsibility for updating the extra node.
   *
   * The extra node cannot be observed, it must not already be in the
   * model graph, it may not have any children, and all of its parents
   * must be in the graph.
   */
  void addExtraNode(Node *node);
  /**
   * Access the list of sampler factories, which is common to all
   * models. This is used during initialization to choose samplers.
   *
   * @seealso Model#chooseSamplers
   */
  static std::list<SamplerFactory const *> &samplerFactories();
  /**
   * Access the list of RNG factories, which is common to all models
   */
  static std::list<RNGFactory *> &rngFactories();
  /**
   * Returns the number of chains in the model
   */
  unsigned int nchain() const;
  /**
   * Returns the RNG object associated with the given chain. If no RNG
   * has been assigned, then a NULL pointer is returned.
   */
  RNG *rng(unsigned int nchain) const;
  /**
   * Assigns a new RNG object to the given chain. The list of
   * RNGFactory objects is traversed and each factory is requested to
   * generate a new RNG object of the given name.
   *
   * @return success indicator.
   */
  bool setRNG(std::string const &name, unsigned int chain);
  /**
   * Assigns an existing RNG object to the given chain
   *
   * @return success indicator
   */
  bool setRNG(RNG *rng, unsigned int chain);
  /**
   * Turns off adaptive phase of all samplers.
   *
   * @return true if all samplers passed the efficiency
   * test. Otherwise false.
   *
   * @see Sampler#adaptOff
   */
  bool adaptOff();
  /**
   * Indicates whether the model is in adaptive mode (before the
   * adaptOff function has been called).
   */
  bool isAdapting() const;
};

/** Returns true if the iteration number is the same for all chains */
bool isSynchronized(Model const *model);

#endif /* MODEL_H_ */
