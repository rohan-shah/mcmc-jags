#include <config.h>
#include <sarray/Range.h>
#include "RangeImp.h"

#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <map>

using std::vector;
using std::map;
using std::pair;
using std::string;
using std::ostringstream;

/* 
   An IndexPair contains sufficient information to identify a Range.

   References are used for efficiency. Since this struct is only used
   privately within this file, we can guarantee that the original
   vectors pointed to by the reference always exist.

   We do not use an STL pair here as it will not accept references.
*/
struct IndexPair {
    vector<int> const &_lower;
    vector<int> const &_upper;
    IndexPair(vector<int> const &lower, vector<int> const &upper)
	: _lower(lower), _upper(upper) {};
};

/*
  Ordering for IndexPair objects based on lexicographic order of upper
  and then lower bound.  

  Two ranges in JAGS are more likely to differ on their upper than
  lower bound (which will normally be 1 for each element), so it is
  more efficient to test the upper bound first.
*/
bool operator<(IndexPair const &l, IndexPair const &r)
{
    if (l._upper < r._upper)
	return true;
    else if (r._upper < l._upper)
	return false;
    else 
	return (l._lower < r._lower);
}

/* 
   ImpMap indexes Ranges by their upper and lower bounds. The global
   _impmap object allows us to access a previously created RangeImp
   object if one has already been created for the corresponding
   IndexPair.

   _impmap is declared inside the getMap accessor function, rather
   than being a global static variable, to avoid problems with
   initialization.
*/
typedef map<IndexPair, RangeImp*> ImpMap;
ImpMap &getMap()
{
    static ImpMap  _impmap;
    return _impmap;
}

/*
  Returns a pointer to a RangeImp object corresponding to the given
  bounds, dynamically allocating a new one if necessary.
*/
static RangeImp *getPtr(vector<int> const &lower, vector<int> const &upper)
{
    ImpMap &impmap = getMap();
    ImpMap::iterator p = impmap.find(IndexPair(lower, upper));
    if (p == impmap.end()) {
	RangeImp * newimp = new RangeImp(lower, upper);
	IndexPair bounds(newimp->lower(), newimp->upper());
	impmap.insert(pair<IndexPair, RangeImp*>(bounds, newimp));
	return newimp;
    }
    else {
	return p->second;
    }
}

Range::Range()
  : _p(getPtr(vector<int>(),vector<int>()))
{
    _p->ref();
}

Range::Range(vector<int> const &lower, vector<int> const &upper)
  : _p(getPtr(lower,upper))
{
    _p->ref();
}

static vector<int> asSigned(vector<unsigned int> const &orig)
{
    unsigned int n = orig.size();
    vector<int> ans(n);
    for (unsigned int i = 0; i < n; ++i) {
	ans[i] = static_cast<int>(orig[i]);
    }
    return ans;
}

Range::Range(vector<unsigned int> const &dim)
  : _p(getPtr(vector<int>(dim.size(), 1), asSigned(dim)))
{
   _p->ref();
}

Range::Range(Range const &rhs)
  : _p(rhs._p)
{
    _p->ref();
}

Range::Range(vector<int> const &index)
  : _p(getPtr(index,index))
{
    _p->ref();
}

Range &Range::operator=(Range const &rhs)
{
    if (_p == rhs._p)
	return *this; //Nothing to do

    if (_p->unRef() == 0) {
	getMap().erase(IndexPair(_p->lower(), _p->upper()));
	delete _p;
    }
    _p = rhs._p;
    _p->ref();
    return *this;
}

Range::~Range()
{
    if (_p->unRef() == 0) {
	getMap().erase(IndexPair(_p->lower(), _p->upper()));
	delete _p;
    }
}

bool Range::operator==(Range const &range) const
{ 
    return _p == range._p; 
}

bool Range::operator!=(Range const &range) const
{ 
    return _p != range._p; 
}

unsigned int Range::length() const 
{ 
    return _p->length(); 
}

bool Range::contains(Range const &range) const 
{ 
    return _p->contains(*range._p); 
}

unsigned int Range::leftOffset(vector<int> const &index) const 
{ 
    return _p->leftOffset(index); 
}

vector<int> Range::leftIndex(unsigned int n) const
{
    return _p->leftIndex(n);
}

unsigned int Range::rightOffset(vector<int> const &index) const 
{ 
    return _p->rightOffset(index); 
}

vector<int> Range::rightIndex(unsigned int n) const
{
    return _p->rightIndex(n);
}

vector<unsigned int> const &Range::dim(bool drop) const 
{ 
    return _p->dim(drop); 
}

unsigned int Range::ndim(bool drop) const 
{ 
    return _p->ndim(drop);
}

vector<int> const & Range::lower() const 
{ 
    return _p->lower(); 
}

vector<int> const & Range::upper() const 
{ 
    return _p->upper(); 
}

bool Range::operator<(Range const &rhs) const 
{ 
    return *_p < *rhs._p; 
}

string print(Range const &range)
{
    ostringstream ostr;
    ostr << "[";
    for (unsigned int i = 0; i < range.ndim(false); ++i) {
	if (i > 0)
	    ostr << ",";
	if (range.lower()[i] == range.upper()[i]) {
	    ostr << range.lower()[i];
	}
	else {
	    ostr << range.lower()[i] << ":" << range.upper()[i];
	}
    }
    ostr << "]";
    return ostr.str();
}
