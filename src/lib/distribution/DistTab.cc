#include <config.h>
#include <distribution/DistTab.h>

#include <functional>
#include <algorithm>

using std::string;
using std::binary_function;
using std::find_if;

typedef std::list<DistPtr> DistList;

// Adaptable binary predicate for find_if algorithm 
struct isDistName: public binary_function<DistPtr, string, bool> 
{
    bool operator()(DistPtr const &dist, string const &name) const
    {
	if (SCALAR(dist))
	    return SCALAR(dist)->name() == name;
	if (VECTOR(dist))
	    return VECTOR(dist)->name() == name;
	if (ARRAY(dist))
	    return ARRAY(dist)->name() == name;
	return false;
    }
};

// Adaptable binary predicate for find_if algorithm 
struct isDistAlias: public binary_function<DistPtr, string, bool> 
{
    bool operator()(DistPtr const &dist, string const &name) const
    {
	if (name.length() == 0)
	    return false;

	if (SCALAR(dist))
	    return SCALAR(dist)->alias() == name;
	if (VECTOR(dist))
	    return VECTOR(dist)->alias() == name;
	if (ARRAY(dist))
	    return ARRAY(dist)->alias() == name;

	return false;
    }
};

void DistTab::insert (DistPtr const &dist)
{
    DistList::const_iterator p = std::find(_dlist.begin(), _dlist.end(), dist);
    if (p == _dlist.end())
	_dlist.push_front(dist);
}

DistPtr const &DistTab::find(string const &name) const
{
    DistList::const_iterator p = 
	find_if(_dlist.begin(), _dlist.end(), bind2nd(isDistName(), name));

    if (p == _dlist.end()) {
	p = find_if(_dlist.begin(), _dlist.end(), bind2nd(isDistAlias(), name));
    }

    return (p == _dlist.end()) ? _nulldist : *p;
}

void DistTab::erase(DistPtr const &dist)
{
    _dlist.remove(dist);
}

