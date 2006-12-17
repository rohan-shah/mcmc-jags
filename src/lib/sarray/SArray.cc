#include <config.h>
#include <sarray/SArray.h>
#include <sarray/nainf.h>

#include <stdexcept>

using std::vector;
using std::logic_error;
using std::length_error;

SArray::SArray(vector<unsigned int> const &dim)
    : _range(dim)
{
    unsigned int n = _range.length();
    _value = new double[n];
    for (unsigned int i = 0; i < n; ++i) {
	_value[i] = JAGS_NA;
    }
    //_fixed = false;
    _discrete = false;
}

SArray::SArray(SArray const &orig)
  : _range(orig._range)
{
    unsigned int n = _range.length();
    _value = new double[n];
    for (unsigned int i = 0; i < n; ++i) {
	_value[i] = orig._value[i];
    }
    //_fixed = false;
    _discrete = orig._discrete;
}

SArray::~SArray()
{
    delete [] _value;
}

Range const &SArray::range() const
{
    return _range;
}

void SArray::setValue(double const *value, unsigned int n)
{
    /*
    if (_fixed) {
	throw logic_error ("Attempt to set value of fixed SArray");
    }
    else 
    */
    if (n != _range.length()) {
	throw length_error("Length mismatch error in SArray::setValue");
    }
    else {
	for (unsigned int i = 0; i < n; ++i) {
	    _value[i] = value[i];
	}
    }
}

void SArray::setValue(double const value, unsigned int i)
{
    /*
    if (_fixed) {
	throw logic_error ("Attempt to set value of fixed SArray");
    }
    else 
    */
    if (i >= _range.length()) {
	throw logic_error("Attempt to set value of invalid element of SArray");
    }
    else {
	_value[i] = value;
    }
}

double const * SArray::value() const
{
    return _value;
}

/*
void SArray::setFixed(bool fix)
{
    if (fix) {
	for (unsigned int i = 0; i < _range.length(); ++i) {
	    if (_value[i] == JAGS_NA)
		throw logic_error("Attempt to fix SArray containing missing values");
	}
    }
    _fixed = fix;
}

bool SArray::isFixed() const
{
    return _fixed;
}
*/

/*
 FIXME: We should be retiring these functions, but need to keep them
 for reading/writing the data table
*/
void SArray::setDiscreteValued(bool discrete)
{
    _discrete = discrete;
}

bool SArray::isDiscreteValued() const
{
    return _discrete;
}
