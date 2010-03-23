#include <config.h>
#include <distribution/DistPtr.h>

using std::string;

DistPtr::DistPtr()
    : sdist(0), vdist(0), adist(0)
{}

DistPtr::DistPtr(ScalarDist const *sf)
    : sdist(sf), vdist(0), adist(0)
{}

DistPtr::DistPtr(VectorDist const *vf)
    : sdist(0), vdist(vf), adist(0)
{}

DistPtr::DistPtr(ArrayDist const *af)
    : sdist(0), vdist(0), adist(af)
{}

string const &DistPtr::name() const
{
    static const string nullstring;
    if (sdist)
	return sdist->name();
    else if (vdist)
	return vdist->name();
    else if (adist)
	return adist->name();
    else
	return nullstring;
}

ScalarDist const * SCALAR(DistPtr const &p) { 
    return p.sdist; 
}

VectorDist const * VECTOR(DistPtr const &p) { 
    return p.vdist; 
}

ArrayDist const * ARRAY(DistPtr const &p) { 
    return p.adist; 
}

bool DistPtr::operator==(DistPtr const &rhs) const
{
    return (sdist==rhs.sdist && vdist==rhs.vdist && adist==rhs.adist);
}

bool isNULL(DistPtr const &p)
{
    return (p.sdist == 0 && p.vdist == 0 && p.adist == 0);
}
