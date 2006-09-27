#include <config.h>
#include "RangeImp.h"

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

static vector<unsigned int> 
makeDim(vector<int> const &lower, vector<int> const &upper)
{
    /* 
       Utility function for constructor: calculates the dimension of the
       range with, or without, dropping of scalar dimensions.
    */
    unsigned int ndim = upper.size();

    if (lower.size() != ndim) {
	throw logic_error("RangeImp: Length mismatch in constructor");
    }
    for (unsigned int i = 0; i < ndim; i++) {
	if (upper[i] < lower[i]) {
	    throw out_of_range("RangeImp: upper < lower bound in constructor");
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

RangeImp::RangeImp(vector<int> const &lower, vector<int> const &upper)
    : _lower(lower), _upper(upper), 
      _dim(makeDim(lower, upper)),
      _dim_dropped(makeDimDropped(_dim)),
      _length(makeLength(_dim)), _ref(0)
{
}

unsigned int RangeImp::length() const
{
    return _length;
}

unsigned int RangeImp::ndim(bool drop) const
{
    return drop ? _dim_dropped.size() : _dim.size();
}

vector<unsigned int> const &RangeImp::dim(bool drop) const
{
    return drop ? _dim_dropped : _dim;
}

bool RangeImp::contains(RangeImp const &other) const
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

bool RangeImp::overlaps(RangeImp const &other) const
{
    unsigned int dim = _upper.size();
    if (other.ndim(false) != dim) {
	throw invalid_argument("RangeImp::overlaps. Dimension mismatch");
    }

    for (unsigned int i = 0; i < dim; i++) {
	if (max(_lower[i], other._lower[i]) > min(_upper[i], other._upper[i])) 
	    return false;
    }
    return true;
}

unsigned int RangeImp::leftOffset(vector<int> const &index) const
{
    if (!contains(RangeImp(index,index))) {
	throw out_of_range("RangeImp::leftOffset. Index outside of allowed range");
    }
  
    unsigned int offset = 0;
    int step = 1;
    unsigned int ndim = _upper.size();
    for (unsigned int i = 0; i < ndim; i++) {
	offset += step * (index[i] - _lower[i]);
	step *= _dim[i];
    }
    return offset;
}

vector<int> RangeImp::leftIndex(unsigned int offset) const
{
    if (offset >= _length) {
	throw out_of_range("RangeImp::leftIndex. Offset exceeds length of range");
    }

    unsigned int ndim = _lower.size();
    vector<int> index(_lower);
    for (unsigned int i = 0; i < ndim; ++i) {
	index[i] += offset % _dim[i];
	offset = offset / _dim[i];
    }
    return index;
}

unsigned int RangeImp::rightOffset(vector<int> const &index) const
{
    if (!contains(RangeImp(index,index))) {
	throw out_of_range("RangeImp::rightOffset. Index outside of allowed range");
    }
 
    unsigned int offset = 0;
    int step = 1;
    for (int i = _upper.size() - 1; i >= 0; --i) {
	offset += step * (index[i] - _lower[i]);
	step *= _dim[i];
    }
    return offset;
}

vector<int> RangeImp::rightIndex(unsigned int offset) const
{
    if (offset >= _length)
	throw out_of_range("RangeImp::rightIndex. Offset exceeds length of range");

    int ndim = _lower.size();
    vector<int> index(_lower);
    for (int i = ndim - 1; i >= 0; --i) {
	index[i] += offset % _dim[i];
	offset = offset / _dim[i];
    }
    return index;
}

vector<int> const & RangeImp::lower() const
{
  return _lower;
}

vector<int> const & RangeImp::upper() const
{
  return _upper;
}

unsigned int RangeImp::ref() 
{
    return ++_ref;
}

unsigned int RangeImp::unRef()
{
    if (_ref > 0) _ref--;
    return _ref;
}

bool RangeImp::operator<(RangeImp const &rhs) const
{
  if (_lower < rhs._lower)
    return true;
  else if (rhs._lower < _lower)
    return false;
  else 
    return _upper < rhs._upper;
}
