#include <config.h>
#include <sarray/RangeIterator.h>

using std::vector;

namespace jags {

    RangeIterator::RangeIterator(Range const &range)
	: vector<int>(range.first()), 
	  _scope(range.scope()),
	  _dim(range.dim(false)),
	  _index(_dim.size(), 0),
	  _atend(0)
    {}

    unsigned int RangeIterator::atEnd() const
    {
	return _atend;
    }

    RangeIterator &RangeIterator::nextLeft()
    {
	unsigned int i = 0;
	for (; i < _index.size(); ++i) {
	    unsigned int &ind = _index[i];
	    int &val = operator[](i);

	    if (++ind >= _dim[i]) ind = 0;
	    val = _scope[i][ind];
	    if (ind > 0) break;
	}
	if (i == _index.size()) {
	    _atend++;
	}
	return *this;
    }

    RangeIterator &RangeIterator::nextRight()
    {
	unsigned int j = _index.size();
	for ( ; j > 0; --j) {
	    int i = j - 1;
	    unsigned int &ind = _index[i];
	    int &val = operator[](i);

	    if (++ind >= _dim[i]) ind = 0;
	    val = _scope[i][ind];
	    if (ind > 0) break;
	}
	if (j == 0) {
	    _atend++;
	}
	return *this;
    }

} //namespace jags
