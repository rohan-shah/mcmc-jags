#ifndef FUNCTION_H_
#define FUNCTION_H_

#include <string>
#include <vector>

class Range;

/**
 * Base class for functions.
 *
 * Function objects contain only constant data members and all member
 * functions are constant. Hence only one object needs to be
 * instantiated for each subclass.
 *
 * The FuncTab class provides a convenient way of storing Function
 * objects and referencing them by name.
 *
 * @see FuncTab
 * @short BUGS-language function or operator
 */
class Function
{
    const std::string _name;
    const unsigned int _npar;
public:
    /**
     * Constructor.
     * @param name name of the function as used in the BUGS language
     * @param npar number of parameters. If npar == 0 then the function
     * takes a variable number of parameters.
     */
    Function(std::string const &name, unsigned int npar);
    virtual ~Function ();
    /**
     * Returns the  BUGS language name of the function
     */
    std::string const &name () const;
   /**
     * Returns an alternate name for the function. The default
     * implementation returns an empty string. However, this function
     * may be overridden to return a non-empty string giving an
     * alternate name for the function.
     */
    virtual std::string alias() const;
    /**
     * Checks that a vector of parameters of length npar is consistent
     * with the function.
     */
    bool checkNPar(unsigned int npar) const;
    /**
     * Returns true if the function returns integer values. The
     * default implementation returns false. A function that returns
     * integer values will need to overload this.
     *
     * @param mask Vector indicating whether parameters are discrete
     * or not.
     */
    virtual bool isDiscreteValued(std::vector<bool> const &mask) const;
    /**
     * Tests whether the function preserves scale transformations, i.e.
     * it can be represented in the form f(x) = B %*% x.
     *
     * It is permitted for the isScale function to return false
     * negatives, i.e. to return false when the function does preserve
     * scale transformations.  Since most functions are not scale
     * transformations, the default method returns false.
     * 
     * @param mask boolean vector of length equal to the number of
     * parameters.  The mask indicates a subset of parameters (those
     * with value true) that are themselves scale transformations of x
     * (the others are not functions of x). At least one element of
     * mask must be true.
     *
     * @param isfixed boolean vector. This may be empty, in which case
     * it is ignored.  A non-empty vector must have length equal to
     * mask, and denotes the parameters whose values are fixed. In
     * this case the test is more restrictive, and returns true only
     * if the coefficient B is fixed, assuming that the parameters for
     * which mask is true are themselves fixed scale transformations.
     *
     * @see DeterministicNode#isClosed
     */
    virtual bool isScale(std::vector<bool> const &mask,
			 std::vector<bool> const &isfixed) const;
    /**
     * Tests whether the function preserves linear transformations, i.e. 
     * a function of the form f(x) = A + B %*% x.
     *
     * All scale functions are linear functions. Therefore, the
     * default method calls isScale. It only needs to be overridden
     * for linear functions that are not scale functions.
     *
     * @see Function#isScale
     */
    virtual bool isLinear(std::vector<bool> const &mask,
			  std::vector<bool> const &isfixed) const;
    /**
     * Tests whether the function preserves power transformations,
     * i.e.  can be expressed as f(x) = a*b^x for some value of a,b,
     * where a,b, and x are all scalar.
     *
     * The default method returns false.
     */
    virtual bool isPower(std::vector<bool> const &mask,
			 std::vector<bool> const &isfixed) const;
    /**
     * Returns a BUGS-language expression representing the function call.
     * The default behaviour for a function named "foo" is to return
     * "foo(arg1,arg2)". Functions that are represented as prefix or infix
     * operators need to override this function.
     *
     * @param par Vector of parameter names for the function
     */
    virtual std::string deparse(std::vector<std::string> const &par) const;
};

#endif /* FUNCTION_H_ */
