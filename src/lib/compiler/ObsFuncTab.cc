#include <config.h>
#include <compiler/ObsFuncTab.h>
#include <function/FunctionPtr.h>
#include <distribution/Distribution.h>

#include <functional>
#include <algorithm>

using std::binary_function;
using std::find_if;

typedef std::pair<Distribution const *, FunctionPtr> ObsFunc;
typedef std::list<ObsFunc> OFList;

struct isDist: public binary_function<ObsFunc, Distribution const *, bool> 
{
    // Adaptable binary predicate for find_if algorithm 
    bool operator()(ObsFunc const &f, Distribution const *dist) const
    {
	return f.first == dist;
    }
};

void ObsFuncTab::insert (Distribution const *dist, FunctionPtr const &func)
{
    ObsFunc f(dist, func);
    if (std::find(_flist.begin(), _flist.end(), f) == _flist.end()) {
	_flist.push_front(f);
    }
}

FunctionPtr const &ObsFuncTab::find(Distribution const *dist) const
{
    OFList::const_iterator p = 
	find_if(_flist.begin(), _flist.end(), bind2nd(isDist(), dist));
    
    return (p == _flist.end()) ? _nullfun : p->second;
}

void ObsFuncTab::erase(Distribution const *dist, FunctionPtr const &func)
{
    ObsFunc f(dist, func);
    OFList::iterator p = std::find(_flist.begin(), _flist.end(), f);
    if (p != _flist.end())
	_flist.erase(p);
}
