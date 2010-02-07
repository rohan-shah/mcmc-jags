#include <config.h>
#include <model/MonitorInfo.h>

using std::string;

MonitorInfo::MonitorInfo(Monitor *monitor, string const &name,
			 Range const &range, string const &type)
    : _monitor(monitor), _name(name), _range(range), _type(type)
{
}

Monitor *MonitorInfo::monitor() const
{
    return _monitor;
}

string const &MonitorInfo::name() const
{
    return _name;
}

string const &MonitorInfo::type() const
{
    return _type;
}

Range const &MonitorInfo::range() const
{
    return _range;
}

bool MonitorInfo::operator==(MonitorInfo const &rhs) const
{
    return (_name == rhs._name && _type == rhs._type && _range == rhs._range
	    && _monitor == rhs._monitor);
}

