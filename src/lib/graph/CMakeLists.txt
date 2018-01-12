add_library(graph OBJECT Node.cc DeterministicNode.cc AggNode.cc MixtureNode.cc MixTab.cc LogicalNode.cc ConstantNode.cc StochasticNode.cc Graph.cc GraphMarks.cc NodeError.cc ScalarLogicalNode.cc LinkNode.cc VectorLogicalNode.cc ArrayLogicalNode.cc VSLogicalNode.cc ScalarStochasticNode.cc VectorStochasticNode.cc ArrayStochasticNode.cc ParentError.cc)
if(NOT WIN32)
	target_compile_options(graph PRIVATE -fPIC)
endif()
