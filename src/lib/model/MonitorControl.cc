#include <config.h>
#include <model/MonitorControl.h>
#include <model/Monitor.h>

#include <stdexcept>

using std::invalid_argument;
using std::string;

MonitorControl::MonitorControl (Monitor *monitor, unsigned int start, 
				unsigned int thin)
    : _monitor(monitor), _start(start), _thin(thin), _niter(0)
{
   if (thin == 0) {
	throw invalid_argument("Illegal thinning interval");
    }
}

unsigned int MonitorControl::start() const
{
    return _start;
}

unsigned int MonitorControl::end() const
{
    return _start + _thin * _niter;
}

unsigned int MonitorControl::niter() const
{
    return _niter;
}

unsigned int MonitorControl::thin() const
{
    return _thin;
}

Monitor const *MonitorControl::monitor() const
{
    return _monitor;
}

void MonitorControl::update(unsigned int iteration)
{
    if (iteration < _start || (iteration - _start) % _thin != 0) {
	return;
    }
    else {
	_monitor->update();
	_niter++;
    }
}

bool MonitorControl::operator==(MonitorControl const &rhs) const
{
    return (_monitor == rhs._monitor &&
	    _start == rhs._start &&
	    _thin == rhs._thin &&
	    _niter == rhs._niter);
}

void MonitorControl::reserve(unsigned int niter)
{
    _monitor->reserve(niter/_thin + 1);
}


