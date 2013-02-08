#ifndef MIX_COMPILER_H_
#define MIX_COMPILER_H_

class Node;
class ParseTree;
class Compiler;

namespace jags {

    Node * getMixtureNode(ParseTree const * var, Compiler *compiler);

}

#endif /* MIX_COMPILER_H_ */
