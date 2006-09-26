#include <config.h>
#include <model/TraceMonitor.h>
#include <graph/Node.h>

#include <stdexcept>
#include <algorithm>

using std::max;
using std::min;
using std::runtime_error;
using std::invalid_argument;
using std::logic_error;

static void align(unsigned int &start, unsigned int &end, unsigned int thin)
{
    /* Ensure that (start % thin == 0) and (end % thin == 0) */

    if (start <= 0)
	start = 1;

    if (end < start)
	end = start;
	
    start = (1 + (start - 1) / thin) * thin;
    end = (end / thin) * thin;
  
    if ((end - start) % thin != 0) {
        throw logic_error("Unable to align start end and thin");
    }
}

TraceMonitor::TraceMonitor(Node const *node, unsigned int start, unsigned int end, unsigned int thin)
{
  _node = node;
  if (thin <= 0) {
    throw invalid_argument("Illegal thinning interval");
  }
  _thin = thin;
  align (start, end, thin);
  _start = start;
  _end = end;
  _size =  (end - start) / thin + 1;
  _values = (double*) calloc(_node->length() * _size, sizeof(double));
  _maxsize = _size;    
  _current = 0;
}

TraceMonitor::TraceMonitor(Node const *node, unsigned int start, unsigned int thin)
{
    const unsigned int initsize = 128;

    _node = node;
    if (thin <= 0) {
      throw invalid_argument("Illegal thinning interval");
    }
    _thin = thin;
    _size = initsize;
    _start = start;
    _end = start + thin * (_size - 1);
    _values = (double*) calloc (node->length() * _size, sizeof(double));
    _maxsize = -1;
    _current = 0;
}

TraceMonitor::~TraceMonitor ()
{
  free (_values);
}

unsigned int TraceMonitor::start() const
{
    return _start;
}

unsigned int TraceMonitor::end() const
{
    return _end;
}

unsigned int TraceMonitor::thin() const
{
    return _thin;
}

unsigned int TraceMonitor::size() const
{
    /* current sample size */
    return _current;
}

unsigned int TraceMonitor::size(unsigned int start, unsigned int end) const
{
  /* sample size between start and end */
  if (end < start)
    return 0;

  if (_current == 0)
    return 0;

  unsigned int last_monitored = _start + (_current - 1) * _thin;    
  align (start, end, _thin);

  if (start <= last_monitored && end >= _start) {
    start = max (start, _start);
    end = min (end, last_monitored);
    return (end - start) / _thin + 1;
  }
  else {
    return 0;
  }
}

void TraceMonitor::update(unsigned int iteration, unsigned int chain)
{
    if (iteration != _start + _thin * _current) {
	return;
    }

    unsigned int node_length = _node->length();

    /* Reallocate vector _values if it is full */
    if (_current == _size && _maxsize == -1) {
      /* Fixme: what do we do if _maxsize != -1 */
      _size = _size * 2;
      _values = (double*) 
	realloc(_values,  node_length * _size * sizeof(double));
      _end = _start + _thin * (_size - 1);
    }
    for (unsigned int i = 0; i < node_length; i++) {
      _values[node_length * _current + i] = _node->value(chain)[i];
    }
    _current++;
}

double const *TraceMonitor::values() const
{
  return _values;
}

Node const *TraceMonitor::node() const
{
  return _node;
}



