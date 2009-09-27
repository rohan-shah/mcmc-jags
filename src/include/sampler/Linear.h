#ifndef LINEAR_H_
#define LINEAR_H_

#include<vector>

class Updater;

/**
 * Helper function to check linearity. The function returns true if
 * all deterministic descendants of the given stochastic nodes (within
 * the given graph) are linear functions.
 *
 * @param snodes Vector of stochastic nodes.
 *
 * @param graph Graph within which testing for linearity takes place.
 * Paths outside this graph are ignored.
 *
 * @pararm fixed Boolean flag. If true, the function checks for fixed
 * linear functions.
 *
 * @param link Boolean flag. If true, then the function tests for a
 * generalized linear model, allowing the last deterministic
 * descendants (i.e. those with no deterministic children) to be link
 * functions.
 *
 * @see Node#isClosed
 */
bool checkLinear(Updater const *updater, bool fixed, bool link=false);

/**
 * Helper function to check for scale transformations. The function
 * returns true if all deterministic descendants of the given node
 * (within the given graph) are scale transformations or scale-mixture
 * transformations.
 *
 * @param snode Node whose descendants are to be checked.
 *
 * @param graph Graph within which testing for linearity takes place.
 * paths outside this grahp are ignored.
 *
 * @pararm fixed Boolean flag. If true, the function checks for fixed
 * scale transformations.
 *
 * @see Node#isClosed
 */
bool checkScale(Updater const *updater, bool fixed);

/**
 * Helper function to check for power transformations. The function
 * returns true if all deterministic descendants of the given node
 * (within the given graph) are power transformations.
 *
 * @param snode Node whose descendants are to be checked.
 *
 * @param graph Graph within which testing takes place.  Paths outside
 * this graph are ignored.
 *
 * @pararm fixed Boolean flag. If true, the function checks for fixed
 * power transformations.
 *
 * @see Node#isClosed
 */
bool checkPower(Updater const *updater, bool fixed);

#endif /* LINEAR_H_ */
