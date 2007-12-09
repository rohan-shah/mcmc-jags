#include <config.h>
#include <sarray/SArray.h>
#include <util/nainf.h>

#include <stdexcept>
#include <algorithm>

using std::vector;
using std::logic_error;
using std::length_error;
using std::copy;
using std::string;

SArray::SArray(vector<unsigned int> const &dim)
    : _range(dim), _value(_range.length(), JAGS_NA), _discrete(false)
{
}

SArray::SArray(SArray const &orig)
    : _range(orig._range), _value(orig._value), _discrete(orig._discrete),
      _dimnames(orig._dimnames)
{
}

Range const &SArray::range() const
{
    return _range;
}

void SArray::setValue(vector<double> const &x)
{
    if (x.size() != _value.size()) {
	throw length_error("Length mismatch error in SArray::setValue");
    }
    else {
        copy(x.begin(), x.end(), _value.begin());
	_discrete = false;
    }
}

void SArray::setValue(vector<int> const &x)
{
    if (x.size() != _value.size()) {
	throw length_error("Length mismatch error in SArray::setValue");
    }
    else {
        copy(x.begin(), x.end(), _value.begin());
	_discrete = true;
    }
}

void SArray::setValue(double value, unsigned int i)
{
    if (i >= _range.length()) {
	throw logic_error("Attempt to set value of invalid element of SArray");
    }
    else {
	_value[i] = value;
    }
}

vector<double> const &SArray::value() const
{
    return _value;
}

bool SArray::isDiscreteValued() const
{
    return _discrete;
}

vector<string> const &SArray::dimNames() const
{
    return _dimnames;
}

void SArray::setDimNames(vector<string> const &names)
{
    if (names.empty() || names.size() == _range.ndim(false)) {
	_dimnames = names;
    }
    else {
	throw length_error("Invalid length in SArray::setDimNames");
    }
}
