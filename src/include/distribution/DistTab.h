#ifndef DIST_TAB_H_
#define DIST_TAB_H_

#include <list>
#include <string>
#include <map>

class Distribution;

/** 
 * @short Look-up table for Distribution objects
 *
 * Since all member functions of the Distribution class are constant,
 * only one instance of any Distribution object is required. The
 * DistTab class provides a means of storing Distribution objects and
 * looking them up by name.
 *
 * @see FuncTab
 */
class DistTab {
    std::list<Distribution const *> _dist_list;
public:
  /**
   * Inserts a distribution into the table.  The same distribution cannot
   * be inserted into the table twice. If this is attempted, the second
   * call to insert will have no effect. Conversely, two different 
   * distributions with the same name in the BUGS language can be inserted.
   * In this case, the second distribution is always returned by 
   * DistTab#find.
   *
   * @param dist Pointer to the distribution to insert. The pointer
   * must be valid for the lifetime of the DistTab, or until the
   * distribution is removed with a call to DistTab#erase.
   */
  void insert(Distribution const *dist);
  /**
   * Finds a distribution by name. If two distributions with the
   * same name have been inserted into the table, the most recently
   * inserted one is returned.
   *
   * @return a pointer to the distribution or 0 if it was not found.
   */
  Distribution const *find(std::string const &name) const;
  /**
   * Removes a distribution from the table. A DistTab does no memory
   * management and will not delete the erased distribution.  This
   * must be done by the owner of the distribution. Conversely only
   * the owner can call the erase function since it requires a non-constant
   * pointer.
   */
  void erase(Distribution *dist);
  /**
   * Returns the list of Distributions inserted into the DistTab.
   */
  std::list<Distribution const *> const &distributions() const;
};

#endif /* DIST_TAB_H_ */
