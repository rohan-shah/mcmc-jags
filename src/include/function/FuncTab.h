#ifndef FUNC_TAB_H_
#define FUNC_TAB_H_

#include <list>
#include <string>

class Function;
class InverseLinkFunc;

/** 
 * The FuncTab class provides a convenient way of storing Function
 * objects and referencing them by name.
 *
 * @short Function table
 * @see Function
 */
class FuncTab
{
    std::list<Function const *> _func_list;
    std::list<Function const *> _masked_func_list;
    std::list<InverseLinkFunc const *> _link_list;
    std::list<InverseLinkFunc const *> _masked_link_list;
public:
  /**
   * Any functions inserted into the FuncTab should be removed before
   * it is destroyed. The destructor therefore prints a warning
   * message to the standard error if the table is non-empty when it
   * is called
   */
  ~FuncTab ();
  /**
   * Inserts an inverse link function into the table. This function
   * works the same way as the standard insert function (see below)
   * but additionally registers the function by its link name 
   * @see InverseLinkFunc
   */
  void insert (InverseLinkFunc const *func);
  /**
   * Inserts a function into the table.
   *
   * @return logical value indicating success.  The insertion will
   * fail if a function with the same name is already in the FuncTab.
   *
   * @param func Pointer to the function to insert, which should
   * be dynamically allocated. The FuncTab is responsible for freeing
   * the memory allocated to func.
   */
  void insert (Function const *func);
  /**
   * Finds a function by name 
   *
   * @return a pointer to the function or 0 if it was not found.
   */
  Function const *find (std::string const &name) const;
  /**
   * Finds the inverse of a link function by the link name
   *
   * @return a pointer to the inverse function or 0 if it was not found.
   */
  Function const *findInverse (std::string const &name) const;
  /**
   * Removes a function from the table. This can only be called by the
   * module that owns the Function object, as it requires a
   * non-constant pointer.
   */
  void erase(InverseLinkFunc *func);
  void erase(Function *func);
};

#endif /* FUNC_TAB_H_ */
