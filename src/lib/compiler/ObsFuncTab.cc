#include <config.h>
#include <compiler/ObsFuncTab.h>
#include <function/FunctionPtr.h>
#include <distribution/Distribution.h>

#include <functional>
#include <algorithm>

using std::binary_function;
using std::find_if;

typedef std::pair<DistPtr, FunctionPtr> ObsFunc;
typedef std::list<ObsFunc> OFList;

struct isDist: public binary_function<ObsFunc, DistPtr, bool> 
{
    // Adaptable binary predicate for find_if algorithm 
    bool operator()(ObsFunc const &f, DistPtr const &dist) const
    {
	return f.first == dist;
    }
};

void ObsFuncTab::insert (DistPtr const &dist, FunctionPtr const &func)
{
    ObsFunc f(dist, func);
    if (std::find(_flist.begin(), _flist.end(), f) == _flist.end()) {
	_flist.push_front(f);
    }
}

FunctionPtr const &ObsFuncTab::find(DistPtr const &dist) const
{
    OFList::const_iterator p = 
	find_if(_flist.begin(), _flist.end(), bind2nd(isDist(), dist));
    
    return (p == _flist.end()) ? _nullfun : p->second;
}

void ObsFuncTab::erase(DistPtr const &dist, FunctionPtr const &func)
{
    _flist.remove(ObsFunc(dist, func));
}
