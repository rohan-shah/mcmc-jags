#include <config.h>
#include <sarray/RangeIterator.h>
#include <stdexcept>

using std::vector;
using std::out_of_range;

RangeIterator::RangeIterator(Range const &range)
  : vector<int>(range.lower()), _range(range), _atend(false)
{
}

RangeIterator::~RangeIterator()
{}

bool RangeIterator::atEnd() const
{
    return _atend;
}

RangeIterator &RangeIterator::nextLeft()
{
    int n = _range.ndim(false);
    int i = 0;
    for (; i < n; ++i) {
        int &ind = operator[](i);
        if (ind < _range.upper()[i]) {
            ++ind;
            break;
        }
        else {
            ind = _range.lower()[i];
        }
    }
    if (i == n) {
        _atend = true;
    }
    return *this;
}

RangeIterator &RangeIterator::nextRight()
{
    int i = _range.ndim(false) - 1;
    for ( ; i >= 0; --i) {
        int &ind = operator[](i);
        if (ind < _range.upper()[i]) {
            ++ind;
            break;
        }
        else {
            ind = _range.lower()[i];
        }
    }
    if (i < 0) {
        _atend = true;
    }
    return *this;
}
