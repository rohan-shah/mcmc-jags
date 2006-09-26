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

struct ltrangeimp
{
  bool operator()(RangeImp const *arg1, RangeImp const *arg2) const
  {
    return *arg1 < *arg2;
  }
};

map<RangeImp const*, unsigned int, ltrangeimp> &impMap()
{
  static map<RangeImp const*, unsigned int, ltrangeimp> _impmap;
  return _impmap;
}

inline unsigned int &ref(RangeImp const *p) { return impMap().find(p)->second; }

static RangeImp const *getPtr(vector<int> const &lower,
                              vector<int> const &upper)
{
  RangeImp * newimp = new RangeImp(lower, upper);  
  map<RangeImp const*, unsigned int, ltrangeimp> &impmap = impMap();
  map<RangeImp const*, unsigned int, ltrangeimp>::iterator p = impmap.find(newimp);
  if (p == impmap.end()) {
    impmap.insert(pair<RangeImp*, unsigned int>(newimp,0));
    return newimp;
  }
  else {
    delete newimp;
    return p->first;
  }
}

Range::Range()
  : _p(getPtr(vector<int>(),vector<int>()))
{
    ref(_p)++;
}

Range::Range(vector<int> const &lower, vector<int> const &upper)
  : _p(getPtr(lower,upper))
{
  ref(_p)++;
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
   ref(_p)++;
}

Range::Range(Range const &rhs)
  : _p(rhs._p)
{
  ref(_p)++;
}

Range::Range(vector<int> const &index)
  : _p(getPtr(index,index))
{
  ref(_p)++;
}

Range &Range::operator=(Range const &rhs)
{
  if (_p == rhs._p)
    return *this; //Nothing to do

  if (--ref(_p) == 0) {
    impMap().erase(_p);
    delete _p;
  }
  _p = rhs._p;
  ref(_p)++;
  return *this;
}

Range::~Range()
{
  if (--ref(_p) == 0) {
    impMap().erase(_p);
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

bool Range::operator<(Range const &rhs) const { 
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
