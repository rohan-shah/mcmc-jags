#include <config.h>
#include <sarray/Range.h>
#include <util/dim.h>

#include <stdexcept>
#include <sstream>

using std::vector;
using std::out_of_range;
using std::logic_error;
using std::string;
using std::ostringstream;

static vector<unsigned int> 
makeDim(vector<vector<int> > const &scope)
{
    vector<unsigned int> dims;
    for (unsigned int i = 0; i < scope.size(); i++) {
	dims.push_back(scope[i].size());
    }
    return dims;
}

namespace jags {

    Range::Range()
	: _scope(0), _dim(0), _dim_dropped(0), _first(0), _last(0), _length()
    {
    }

    Range::Range(vector<vector<int> > const &scope)
	: _scope(scope),
	  _dim(makeDim(_scope)), 
	  _dim_dropped(drop(_dim)),
	  _first(0),
	  _last(0),
	  _length(product(_dim))
    {
	//Check that we have no zero-length index vectors.  The
	//leftIndex and rightIndex member functions rely on this
	//assumption, as well as the RangeIterator class.

	for (unsigned int i = 0; i < scope.size(); ++i) {
	    if (scope[i].empty()) {
		throw logic_error("Zero dimension in Range constructor");
	    }
	    _first.push_back(scope[i].front());
	    _last.push_back(scope[i].back());
	}
    }

    Range::~Range()
    {
	//Virtual destructor
    }

    unsigned int Range::length() const
    {
	return _length;
    }

    unsigned int Range::ndim(bool drop) const
    {
	return drop ? _dim_dropped.size() : _dim.size();
    }

    vector<unsigned int> const &Range::dim(bool drop) const
    {
	return drop ? _dim_dropped : _dim;
    }
    
    vector<int> Range::leftIndex(unsigned int offset) const
    {
	if (offset >= _length) {
	    throw out_of_range("Range::leftIndex. Offset out of bounds");
	}
	
	unsigned int ndim = _scope.size();
	vector<int> index(ndim);
	for (unsigned int i = 0; i < ndim; ++i) {
	    index[i] = _scope[i][offset % _dim[i]];
	    offset /= _dim[i];
	}
	return index;
    }

    vector<int> Range::rightIndex(unsigned int offset) const
    {
	if (offset >= _length) {
	    throw out_of_range("Range::rightIndex. Offset out of bounds");
	}

	int ndim = _scope.size();
	vector<int> index(ndim);
	for (int i = ndim - 1; i >= 0; --i) {
	    index[i] = _scope[i][offset % _dim[i]];
	    offset /= _dim[i];
	}
	return index;
    }

    vector<int> const &Range::first() const
    {
	return _first;
    }

    vector<int> const &Range::last() const
    {
	return _last;
    }

    bool Range::operator==(Range const &range) const
    {
	return _scope == range._scope;
    }

    bool Range::operator!=(Range const &range) const
    {
	return _scope != range._scope;
    }

    bool Range::operator<(Range const &rhs) const
    {
	//Sort by the first element
	if (_first < rhs._first) {
	    return true;
	}
	else if (rhs._first < _first) {
	    return false;
	}
	//Then last element
	else if (_last < rhs._last) {
	    return true;
	}
	else if (rhs._last < _last) {
	    return false;
	}
	//Then lexigraphic ordering of the scope
	return _scope < rhs._scope;
    }
	
	
    string print(Range const &range)
    {
	if (isNULL(range))
	    return "";
	
	ostringstream ostr;
	ostr << "[";
	for (unsigned int i = 0; i < range.ndim(false); ++i) {
	    if (i > 0) {
		ostr << ",";
	    }
	    vector<int> const &indices = range.scope()[i];
	    ostr << indices[0];
	    if (indices.size() > 1) {
		bool simple = true;
		int val = indices[0] + 1;
		for (unsigned int j = 1; j < indices.size(); ++j) {
		    if (indices[j] != val) {
			simple = false;
			break;
		    }
		    ++val;
		}
		if (simple) {
		    ostr << ":";
		}
		else {
		    ostr << "...";
		}
		ostr << indices.back();
	    }
	}
	ostr << "]";
	return ostr.str();
    }

    vector<vector<int> > const &Range::scope() const
    {
	return _scope;
    }

} //namespace jags
