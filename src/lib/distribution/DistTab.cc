#include <config.h>
#include <distribution/DistTab.h>
#include <distribution/Distribution.h>
#include <sarray/SArray.h>

#include <iostream>

using std::string;
using std::list;
typedef std::map<const string, Distribution const *> DistMap;

void DistTab::insert(Distribution const *dist)
{
    string const &name = dist->name();
    DistMap::iterator p = _table.find(name);
    if (p != _table.end()) {
	_table.erase(p);
    }
    _table[name] = dist;
}

Distribution const *DistTab::find(string const &name) const
{
    DistMap::const_iterator p = _table.find(name);

    return (p == _table.end()) ? 0 : p->second;
}

void DistTab::erase(Distribution *dist)
{
    DistMap::iterator p = _table.find(dist->name());
    if (p != _table.end() && p->second == dist) {
	_table.erase(p);
    }
    
}
