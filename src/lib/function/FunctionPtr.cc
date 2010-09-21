#include <config.h>
#include <function/FunctionPtr.h>

using std::string;

FunctionPtr::FunctionPtr()
    : lfunc(0), sfunc(0), vfunc(0), afunc(0)
{}

FunctionPtr::FunctionPtr(ScalarFunction const *sf)
    : lfunc(0), sfunc(sf), vfunc(0), afunc(0)
{}

FunctionPtr::FunctionPtr(VectorFunction const *vf)
    : lfunc(0), sfunc(0), vfunc(vf), afunc(0)
{}

FunctionPtr::FunctionPtr(ArrayFunction const *af)
    : lfunc(0), sfunc(0), vfunc(0), afunc(af)
{}

FunctionPtr::FunctionPtr(LinkFunction const *lf)
    : lfunc(lf), sfunc(0), vfunc(0), afunc(0)
{}

string const &FunctionPtr::name() const
{
    static const string nullstring;
    if (lfunc)
	return lfunc->name();
    else if (sfunc)
	return sfunc->name();
    else if (vfunc)
	return vfunc->name();
    else if (afunc)
	return afunc->name();
    else
	return nullstring;
}

LinkFunction const * LINK(FunctionPtr const &p) { 
    return p.lfunc; 
}

ScalarFunction const * SCALAR(FunctionPtr const &p) { 
    return p.sfunc; 
}

VectorFunction const * VECTOR(FunctionPtr const &p) { 
    return p.vfunc; 
}

ArrayFunction const * ARRAY(FunctionPtr const &p) { 
    return p.afunc; 
}

Function const * FUNC(FunctionPtr const &p) 
{
    if (p.lfunc)
	return p.lfunc;
    else if (p.sfunc)
	return p.sfunc;
    else if (p.vfunc)
	return p.vfunc;
    else if (p.afunc)
	return p.afunc;
    else
	return 0;
}

bool FunctionPtr::operator==(FunctionPtr const &rhs) const
{
    return (lfunc == rhs.lfunc && sfunc==rhs.sfunc && vfunc==rhs.vfunc &&
	    afunc==rhs.afunc);
}
	

bool isNULL(FunctionPtr const &p)
{
    return (p.lfunc==0 && p.sfunc == 0 && p.vfunc == 0 && p.afunc == 0);
}
