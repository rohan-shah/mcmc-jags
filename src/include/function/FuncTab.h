#ifndef FUNC_TAB_H_
#define FUNC_TAB_H_

#include <list>
#include <string>

class InverseLinkFunc;
class Function;

/**
 * @short Look-up table for Function objects
 *
 * Since all member functions of the JAGS Function class are constant, only
 * one instance of a Function object is required. The FuncTab class 
 * provides a means of storing Functions and looking them up by name.
 *
 * @see Function DistTab 
 */
class FuncTab
{
    std::list<Function const *> _func_list;
    std::list<Function const *> _masked_func_list;
    std::list<InverseLinkFunc const *> _link_list;
    std::list<InverseLinkFunc const *> _masked_link_list;
public:
  /**
   * Inserts an inverse link function into the table. This function
   * works the same way as the standard insert function (see below)
   * but additionally registers the function by its link name.
   */
  void insert (InverseLinkFunc const *func);
  /**
   * Inserts a function into the table. This function will mask any
   * function of the same name previously inserted into the table,
   * making them inaccessible until this function is erased.
   *
   * @param func Function to insert into the table.
   */
  void insert (Function const *func);
  /**
   * Finds a function by name.  If more than one function with the
   * same name has been inserted into the table, the most recently
   * inserted Function will be returned.
   *
   * @return a pointer to the function or a NULL pointer if it was not
   * found.
   */
  Function const *find (std::string const &name) const;
  /**
   * Finds the inverse of a link function by name
   *
   * @parameter name to search on.
   *
   * @link_name Logical flag. If true then search by link name
   * (e.g. "log"), otherwise search by the name of the inverse (e.g. "exp")
   *
   * @return a pointer to the inverse link function or a NULL pointer if it
   * was not found.
   */
  InverseLinkFunc const *findInverseLink (std::string const &name,
					  bool link_name) const;
  /**
   * Removes an inverse link function from the table
   */
  void erase(InverseLinkFunc *func);
  /**
   * Removes a function from the table. This can only be called by the
   * module that owns the Function object, as it requires a
   * non-constant pointer.
   */
  void erase(Function *func);
};

#endif /* FUNC_TAB_H_ */
