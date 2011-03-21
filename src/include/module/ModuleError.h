#ifndef MODULE_ERROR_H_
#define MODULE_ERROR_H_

#include <string>

class Node;

/**
 * When an error occurs in a module instead of throwing an exception
 * it sets the value of some global variables which can later be used
 * to throw an exception within the JAGS library.
 *
 * This 2-step process is necessary because on Windows, JAGS is often
 * statically linked against the C++ runtime library - for
 * compatibility with R.  As a consequence, it is not possible to
 * throw exceptions across DLL boundaries.
 * 
 * The setModuleError allows the module to set an error message and,
 * optionally, the relevant node.
 *
 * @param message String giving an informative error message to the user
 * @param node Optional pointer to the node where the problem occurred
 */
void setModuleError(std::string const &message, Node const *node = 0);

/**
 * This utility function is called by the JAGS library after an error
 * message has been set with the setModuleError function. It will
 * throw an exception based on the information provided to
 * setModuleError. 
 *
 * If setModuleError has not been called, or if it has been called
 * with an empty string for a message, then throwModuleError will do
 * nothing.
 */
void throwModuleError();

#endif /* MODULE_ERROR_H_ */
