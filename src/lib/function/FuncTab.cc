#include <config.h>
#include <function/FuncTab.h>
#include <function/Function.h>
#include <function/InverseLinkFunc.h>

#include <functional>
#include <iostream>
#include <algorithm>

using std::list;
using std::string;
using std::binary_function;
using std::find_if;

typedef std::list<Function const*> FuncList;
typedef std::list<InverseLinkFunc const*> LinkList;

/* Adaptable binary predicate for find_if algorithm */
struct isFuncName: 
    public binary_function<Function const *, string const *, bool> 
{
    bool operator()(Function const *func, string const *name) const
    {
	return func->name() == *name;
    }
};

/* Adaptable binary predicate for find_if algorithm */
struct isLinkName: 
    public binary_function<InverseLinkFunc const *, string const *, bool> 
{
    bool operator()(InverseLinkFunc const *lfunc, string const *name) const
    {
	return lfunc->linkName() == *name;
    }
};

void FuncTab::insert (Function const *func)
{
    FuncList::iterator p = std::find(_func_list.begin(), _func_list.end(),
				     func);
    if (p == _func_list.end()) {
	_func_list.push_front(func);
    }
}

void FuncTab::insert (InverseLinkFunc const *lfunc)
{
    FuncList::iterator p = std::find(_func_list.begin(), _func_list.end(),
				     lfunc);
    LinkList::iterator q = std::find(_link_list.begin(), _link_list.end(),
				     lfunc);

    if (p == _func_list.end() && q == _link_list.end()) {
	_func_list.push_front(lfunc);
	_link_list.push_front(lfunc);
    }
}

Function const * FuncTab::find (string const &funcname) const
{
    //Note scoping operator to distinguish std::find from FuncTab::find
    FuncList::const_iterator p = std::find_if(_func_list.begin(), 
					      _func_list.end(),
					      bind2nd(isFuncName(), &funcname));

    return (p == _func_list.end()) ? 0 : *p;
}

InverseLinkFunc const * FuncTab::findInverseLink (string const &name,
						  bool link_name) const
{

    LinkList::const_iterator p = _link_list.end();
    if (link_name) {
	//Note scoping operator to distinguish std::find from FuncTab::find
	p = std::find_if(_link_list.begin(), _link_list.end(),
			 bind2nd(isLinkName(), &name));
    }
    else {
	p = std::find_if(_link_list.begin(), _link_list.end(),
			 bind2nd(isFuncName(), &name));
    }
    return (p == _link_list.end()) ? 0 : *p;
}

void FuncTab::erase(Function *func)
{
    FuncList::iterator p = std::find(_func_list.begin(), _func_list.end(),
				     func);
    if (p == _func_list.end())
	return;


    //Erase from the function list.
    _func_list.erase(p);

    //If it's a link function, also erase it from the link list
    for (LinkList::iterator p = _link_list.begin(); p != _link_list.end(); ++p)
    {
	if (func == *p) {
	    _link_list.erase(p);
	    break;
	}
    }
}
