#include <config.h>
#include <sarray/RangeIterator.h>
#include <stdexcept>

using std::vector;

RangeIterator::RangeIterator(Range const &range)
    : vector<int>(range.lower()), _lower(range.lower()), _upper(range.upper()),
      _atend(0)
{}

unsigned int RangeIterator::atEnd() const
{
    return _atend;
}

RangeIterator &RangeIterator::nextLeft()
{
    int n = _lower.size();
    int i = 0;
    for (; i < n; ++i) {
        int &ind = operator[](i);
        if (ind < _upper[i]) {
            ++ind;
            break;
        }
        else {
            ind = _lower[i];
        }
    }
    if (i == n) {
        _atend++;
    }
    return *this;
}

RangeIterator &RangeIterator::nextRight()
{
    int i = _lower.size() - 1;
    for ( ; i >= 0; --i) {
        int &ind = operator[](i);
        if (ind < _upper[i]) {
            ++ind;
            break;
        }
        else {
            ind = _lower[i];
        }
    }
    if (i < 0) {
        _atend++;
    }
    return *this;
}
