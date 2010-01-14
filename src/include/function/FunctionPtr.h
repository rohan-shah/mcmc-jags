#ifndef FUNCTION_POINTER_H_
#define FUNCTION_POINTER_H_

#include <function/InverseLinkFunc.h>
#include <function/ScalarFunction.h>
#include <function/VectorFunction.h>
#include <function/ArrayFunction.h>

/**
 * @short Polymorphic pointer to Function
 *
 * A FunctionPtr holds a pointer to one of the four sub-classes of
 * Function: InverseLinkFunc, ScalarFunction, VectorFunction, or
 * ArrayFunction.  The pointers can be extracted using the extractor
 * functions LINK, SCALAR, VECTOR, and ARRAY, respectively . 
 */
class FunctionPtr
{
    InverseLinkFunc const * lfunc;
    ScalarFunction const * sfunc;
    VectorFunction const * vfunc;
    ArrayFunction const * afunc;
public:
    FunctionPtr();
    FunctionPtr(ScalarFunction const *);
    FunctionPtr(VectorFunction const *);
    FunctionPtr(ArrayFunction const *);
    FunctionPtr(InverseLinkFunc const *);
    bool operator==(FunctionPtr const &rhs) const;
    std::string const &name() const;
    friend InverseLinkFunc const *LINK(FunctionPtr const &p);
    friend ScalarFunction const *SCALAR(FunctionPtr const &p);
    friend VectorFunction const *VECTOR(FunctionPtr const &p);
    friend ArrayFunction const *ARRAY(FunctionPtr const &p);
    friend bool isNULL(FunctionPtr const &p);
};

#endif /* FUNCTION_POINTER_H_ */
