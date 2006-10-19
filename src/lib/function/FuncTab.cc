#include <config.h>
#include <function/FuncTab.h>
#include <function/Function.h>
#include <function/InverseLinkFunc.h>

#include <functional>
#include <iostream>

using std::list;
using std::string;
using std::binary_function;

typedef std::list<Function const*> FuncList;
typedef std::list<InverseLinkFunc const*> LinkList;

//FIXME: We should be using templates here

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

FuncTab::~FuncTab ()
{

}

bool FuncTab::insert (Function const *func)
{
    FuncList::iterator p = find_if(_func_list.begin(), _func_list.end(),
                                   bind2nd(isFuncName(), &func->name()));

    //Transfer function with same name (if any) to the masked list
    if (p != _func_list.end()) {
	_masked_func_list.push_front(*p);
	_func_list.erase(p);
    }
    _func_list.push_front(func);

    /*
      string const &name = func->name ();
      if (_table.find (name) == _table.end ()) {
      _table[name] = func;
      return true;
      }
      else {
      return false;
      }
    */
}

bool FuncTab::insert (InverseLinkFunc const *lfunc)
{
    FuncList::iterator p = find_if(_func_list.begin(), _func_list.end(),
                                   bind2nd(isFuncName(), &lfunc->name()));

    //Transfer function with same name (if any) to the masked list
    if (p != _func_list.end()) {
	_masked_func_list.push_front(*p);
	_func_list.erase(p);
    }
    _func_list.push_front(lfunc);

    LinkList::iterator q = find_if(_link_list.begin(), _link_list.end(),
				   bind2nd(isLinkName(), &lfunc->linkName()));
				   
    //Transfer link function with same name (if any) to the masked list
    if (q != _link_list.end()) {
	_masked_link_list.push_front(*q);
	_link_list.erase(q);
    }
    _link_list.push_front(lfunc);


    /*
    string const &name = lfunc->name ();
    string const &linkname = lfunc->linkName ();
    if (_link_table.find (linkname) == _link_table.end () &&
	_table.find (name) == _table.end ())
    {
	_table[name] = lfunc;
	_link_table[linkname] = lfunc;

	return true;
    }
    else
    {
	return false;
    }
    */

}

Function const * FuncTab::find (string const &funcname) const
{
    //Note scoping operator to distinguish std::find from FuncTab::find
    FuncList::const_iterator p = std::find_if(_func_list.begin(), 
					      _func_list.end(),
					      bind2nd(isFuncName(), &funcname));

    return (p == _func_list.end()) ? 0 : *p;

    /*
    map < const string, const Function *>::const_iterator p 
	= _table.find (name);

    if (p == _table.end ()) {
	return 0;
    }
    else {
	return p->second;
    }
    */
}

Function const * FuncTab::findInverse (string const &linkname) const
{

    //Note scoping operator to distinguish std::find from FuncTab::find
    LinkList::const_iterator p = std::find_if(_link_list.begin(), 
					      _link_list.end(),
					      bind2nd(isLinkName(), &linkname));
    
    return (p == _link_list.end()) ? 0 : *p;

    /*
    map < const string, const InverseLinkFunc *>::const_iterator 
	p =_link_table.find (name);
  
    if (p == _link_table.end ()) {
	return 0;
    }
    else {
	return p->second;
    }
    */
}


void FuncTab::erase(Function *func)
{
    /*
    string const &name = func->name();
    map<const string, const Function*>::iterator p = _table.find(name);
    if (p != _table.end()) {
	if (p->second == func) {
	    erase(name);
	}
    }
    */

    //Erase from the main list.
    FuncList::iterator p = std::find(_func_list.begin(), _func_list.end(),
				     func);
    bool move_masked = false;
    if (p != _func_list.end()) {
	_func_list.erase(p);
	move_masked = true; //Move any masked funcributions into the main list
    }

    //Erase from the masked list
    p = std::find(_masked_func_list.begin(), _masked_func_list.end(), func);
    if (p != _masked_func_list.end()) {
        if (move_masked) {
	    _func_list.push_front(*p);
	}
	_masked_func_list.erase(p);
    }
}


void FuncTab::erase(InverseLinkFunc *link)
{
    //Erase from the main list.
    LinkList::iterator p = std::find(_link_list.begin(), _link_list.end(),
				     link);
    bool move_masked = false;
    if (p != _link_list.end()) {
	_link_list.erase(p);
	move_masked = true; //Move any masked link functions into the main list
    }

    //Erase from the masked list
    p = std::find(_masked_link_list.begin(), _masked_link_list.end(), link);
    if (p != _masked_link_list.end()) {
        if (move_masked) {
	    _link_list.push_front(*p);
	}
	_masked_link_list.erase(p);
    }
}
