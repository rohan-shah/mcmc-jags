#ifndef DIST_TAB_H_
#define DIST_TAB_H_

#include <map>
#include <list>
#include <string>

class Distribution;

/** 
 * A DistTab is a table which can be used to look up distributions by
 * name. A DistTab does no memory management: the module responsible
 * for creating any Distribution inserted into the table is also
 * responsible for deleting it.
 *
 * @short Distribution table 
 * 
 * @see Distribution
 */
class DistTab {
  std::map<const std::string, const Distribution*> _table;
  std::list<Distribution *> _overloaded;
public:
  /**
   * Inserts a distribution into the table.  The new distribution will
   * replace any existing distribution with the same name in the table.
   *
   * @param dist Pointer to the distribution to insert. The pointer
   * must be valid for the lifetime of the DistTab, unless it is 
   * removed with a call to erase.
   */
  void insert(Distribution const *dist);
  /**
   * Finds a distribution by name
   *
   * @return a pointer to the distribution or 0 if it was not found.
   */
  Distribution const *find(std::string const &name) const;
  /**
   * Removes a distribution, if it is contained in the table: does
   * nothing if the distribution is not in the table.
   *
   * In practice, this function can only be called by the module that
   * created the distribution, since it requires a non-constant
   * pointer
   */
  void erase(Distribution *dist);
};

#endif /* DIST_TAB_H_ */
