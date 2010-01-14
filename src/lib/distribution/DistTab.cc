#include <config.h>
#include <distribution/DistTab.h>
#include <distribution/Distribution.h>
#include <functional>
#include <algorithm>

using std::string;
using std::binary_function;
using std::find_if;
using std::pair;

typedef std::list<Distribution const*> DistList;

/* 
   Adaptable binary predicate for find_if algorithm to allow functions
   to be found by name
*/
struct isName: 
    public binary_function<Distribution const *, string, bool> 
{
    bool operator()(Distribution const *dist, string const &name) const
    {
	return dist->name() == name;
    }
};

void DistTab::insert(Distribution const *dist)
{
    DistList::const_iterator p = std::find(_dist_list.begin(), _dist_list.end(),				      dist);

    if (p == _dist_list.end()) {
	_dist_list.push_front(dist);
    }
}

Distribution const *DistTab::find(string const &name) const
{
    DistList::const_iterator p = find_if(_dist_list.begin(), _dist_list.end(),
					 bind2nd(isName(), name));
    
    return (p == _dist_list.end()) ? 0 : *p;
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

