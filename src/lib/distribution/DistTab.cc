#include <config.h>
#include <distribution/DistTab.h>
#include <distribution/Distribution.h>
#include <function/Function.h>
#include <functional>
#include <algorithm>

using std::string;
using std::binary_function;
using std::find_if;
using std::pair;

typedef std::list<Distribution const*> DistList;
typedef std::map<Distribution const*, Function const*> FuncMap;

/* 
   Adaptable binary predicate for find_if algorithm to allow functions
   to be found by name
*/
struct isName: 
    public binary_function<Distribution const *, string const *, bool> 
{
    bool operator()(Distribution const *dist, string const *name) const
    {
	return dist->name() == *name;
    }
};

void DistTab::insert(Distribution const *dist)
{
    DistList::const_iterator p = std::find(_dist_list.begin(), _dist_list.end(),				      dist);

    if (p == _dist_list.end()) {
	_dist_list.push_front(dist);
    }
}

void DistTab::insert(Distribution const *dist, Function const *func)
{
    if (dist->name() == func->name()) {
	insert(dist);
	_func_map.insert(pair<Distribution const*,Function const*>(dist,func));
    }
}

Distribution const *DistTab::find(string const &distname) const
{
    DistList::const_iterator p = find_if(_dist_list.begin(), 
					 _dist_list.end(),
					 bind2nd(isName(), &distname));
    
    return (p == _dist_list.end()) ? 0 : *p;
}

Function const *DistTab::findFunction(Distribution const *dist) const
{
    FuncMap::const_iterator p = _func_map.find(dist);
    return (p == _func_map.end()) ? 0 : p->second;
}

void DistTab::erase(Distribution *dist)
{
    DistList::iterator p = std::find(_dist_list.begin(), _dist_list.end(), 
				     dist);
    if (p != _dist_list.end()) {
	_dist_list.erase(p);
    }
}

DistList const &DistTab::distributions() const
{
    return _dist_list;
}

