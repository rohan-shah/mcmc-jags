#include <config.h>
#include <sarray/Range.h>

#include <algorithm>
#include <stdexcept>
#include <sstream>

using std::vector;
using std::min;
using std::max;
using std::out_of_range;
using std::invalid_argument;
using std::logic_error;
using std::string;
using std::ostringstream;


static vector<int> asSigned(vector<unsigned int> const &orig)
{
    unsigned int n = orig.size();
    vector<int> ans(n);
    for (unsigned int i = 0; i < n; ++i) {
        ans[i] = static_cast<int>(orig[i]);
    }
    return ans;
}

static vector<unsigned int> 
makeDim(vector<int> const &lower, vector<int> const &upper)
{
    /* 
       Utility function for constructor: calculates the dimension of the
       range with, or without, dropping of scalar dimensions.
    */
    unsigned int ndim = upper.size();

    if (lower.size() != ndim) {
	throw logic_error("Range: Length mismatch in constructor");
    }
    for (unsigned int i = 0; i < ndim; i++) {
	if (upper[i] < lower[i]) {
	    throw out_of_range("Range: upper < lower bound in constructor");
	}
    }
    
    vector<unsigned int> dim(ndim);
    for (unsigned int i = 0; i < ndim; i++) {
	dim[i] = static_cast<unsigned int>(upper[i] - lower[i] + 1);
    }
    return dim;
}

static vector<unsigned int> makeDimDropped(vector<unsigned int> const &dim)
{
    /* 
       Utility function for constructor. Reduces the dimensions of 
       an array by dropping scalar dimensions.
    */

    unsigned int ndim = dim.size();
    unsigned int size = 0;
    for (unsigned int i = 0; i < ndim; i++) {
	size += (dim[i] > 1);
    }
    if (size == ndim) {
	// No scalar dimensions OR null dimension
	return dim;
    }
    else if (size == 0) {
	// All dimensions are scalar. Keep one of them.
	return vector<unsigned int>(1,1);
    }
    else {
	vector<unsigned int> newdim(size);
	unsigned int j = 0;
	for (unsigned int i = 0; i < ndim; i++) {
	    if (dim[i] > 1) {
		newdim[j++] = dim[i];
	    }
	}
	return newdim;
    }
}

static unsigned int 
makeLength(vector<unsigned int> const &dim)
{
    /* 
       Utility function for constructor: calculates the length of the
       range based on its dimensions
    */
    unsigned int ndim = dim.size();
    if (ndim == 0) {
	return 0;
    }
    else {
	unsigned int length = 1;
	for (unsigned int i = 0; i < ndim; i++) {
	    length *= dim[i];
	}
	return length;
    }
}

Range::Range()
  : _lower(0), _upper(0), _dim(0), _dim_dropped(0), _length(0)
{
}

Range::Range(vector<int> const &lower, vector<int> const &upper)
    : _lower(lower), _upper(upper), 
      _dim(makeDim(lower, upper)),
      _dim_dropped(makeDimDropped(_dim)),
      _length(makeLength(_dim))
{
}

Range::Range(vector<int> const &index)
    : _lower(index), _upper(index), 
      _dim(makeDim(index,index)),
      _dim_dropped(makeDimDropped(_dim)),
      _length(makeLength(_dim))
{
}

Range::Range(vector<unsigned int> const &dim)
    : _lower(vector<int>(dim.size(),1)), _upper(asSigned(dim)), 
      _dim(makeDim(_lower,_upper)),
      _dim_dropped(makeDimDropped(_dim)),
      _length(makeLength(_dim))
{
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

bool Range::contains(Range const &other) const
{
    unsigned int ndim = _upper.size();
    if (other._lower.size() != ndim) {
	throw invalid_argument("Range::contains. Dimension mismatch");
    }
  
    for (unsigned int i = 0; i < ndim; ++i) {
	if (other._lower[i] < _lower[i] || other._upper[i] > _upper[i]) {
	    return false;
	}
    }
    return true;
}

/*
bool Range::overlaps(Range const &other) const
{
    unsigned int dim = _upper.size();
    if (other.ndim(false) != dim) {
	throw invalid_argument("Range::overlaps. Dimension mismatch");
    }

    for (unsigned int i = 0; i < dim; i++) {
	if (max(_lower[i], other._lower[i]) > min(_upper[i], other._upper[i])) 
	    return false;
    }
    return true;
}
*/

unsigned int Range::leftOffset(vector<int> const &index) const
{
    unsigned int offset = 0;
    int step = 1;
    unsigned int ndim = _upper.size();
    for (unsigned int i = 0; i < ndim; i++) {
	if (index[i] < _lower[i] || index[i] > _upper[i]) {
	    throw out_of_range("Range::leftOffset. Index outside of allowed range");
	}
	offset += step * (index[i] - _lower[i]);
	step *= _dim[i];
    }
    return offset;
}

vector<int> Range::leftIndex(unsigned int offset) const
{
    if (offset >= _length) {
	throw out_of_range("Range::leftIndex. Offset exceeds length of range");
    }

    unsigned int ndim = _lower.size();
    vector<int> index(_lower);
    for (unsigned int i = 0; i < ndim; ++i) {
	index[i] += offset % _dim[i];
	offset = offset / _dim[i];
    }
    return index;
}

unsigned int Range::rightOffset(vector<int> const &index) const
{
    unsigned int offset = 0;
    int step = 1;
    for (int i = _upper.size() - 1; i >= 0; --i) {
	if (index[i] < _lower[i] || index[i] > _upper[i]) {
	    throw out_of_range("Range::rightOffset. Index outside of allowed range");
	}
	offset += step * (index[i] - _lower[i]);
	step *= _dim[i];
    }
    return offset;
}

vector<int> Range::rightIndex(unsigned int offset) const
{
    if (offset >= _length)
	throw out_of_range("Range::rightIndex. Offset exceeds length of range");

    int ndim = _lower.size();
    vector<int> index(_lower);
    for (int i = ndim - 1; i >= 0; --i) {
	index[i] += offset % _dim[i];
	offset = offset / _dim[i];
    }
    return index;
}

vector<int> const & Range::lower() const
{
  return _lower;
}

vector<int> const & Range::upper() const
{
  return _upper;
}

bool Range::operator<(Range const &rhs) const
{
  if (_lower < rhs._lower)
    return true;
  else if (rhs._lower < _lower)
    return false;
  else 
    return _upper < rhs._upper;
}

bool Range::operator==(Range const &range) const
{
    return (_lower == range._lower) && (_upper == range._upper);
}

bool Range::operator!=(Range const &range) const
{
    return (_lower != range._lower) || (_upper != range._upper);
}

string print(Range const &range)
{
    if (isNULL(range))
        return "";

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

