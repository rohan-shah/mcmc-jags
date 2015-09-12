#include <config.h>
#include <graph/ParentError.h>

#include <graph/Node.h>
#include <model/SymTab.h>
#include <util/dim.h>
#include <sarray/RangeIterator.h>
#include <sarray/SimpleRange.h>

using std::vector;
using std::string;
using std::ostream;
using std::endl;


/* Print a vector as a sequence of space-separated values */
static void printVector(ostream &out, double const *values, unsigned int length)
{
    for (unsigned int i = 0; i < length; ++i) {
	out << " " << values[i];
    }
    out << "\n";
}

/* Print a matrix by row, bearing in mind that the values are stored
   in column-major order */
static void printMatrix(ostream &out, double const *values, 
			unsigned int nrow, unsigned int ncol)
{
    out << "\n";
    for (unsigned int r = 0; r < nrow; ++r) {
	for (unsigned int c = 0; c < ncol; ++c) {
	    out << " " << values[c * nrow + r];
	}
	out << "\n";
    }
}

/* Print a multi-dimensional array as a sequence of matrices,
   each representing a 2-D slice through the array
*/
static void printArray(ostream &out, double const *value,
		       vector<unsigned int> const &dim)
{
    if (dim.size() <= 2) return;

    jags::SimpleRange range(dim);
    jags::RangeIterator r(range); 
    while(!r.atEnd()) {
	//Header for each slice
	out << " , ";
	for (unsigned int i = 2; i < dim.size(); ++i) {
	    out << ", " << r[i];
	}
	//Print this slice
	printMatrix(out, value + range.leftOffset(r), dim[0], dim[1]);
	//Go to the next slice
	do {
	    r.nextLeft();
	} 
	while(r[0] != 1 || r[1] != 1);
    }
}

namespace jags {

    ParentError::ParentError(Node const *node, unsigned int chain)
	: runtime_error("Invalid parent values"), _node(node), _chain(chain)
    {
    }
    
    void ParentError::printMessage(ostream &out, SymTab const &symtab) const
    {
	out << "Error in node " <<  symtab.getName(_node) << "\n" 
	    << "Invalid parent values" << "\n";

	for (vector<Node const*>::const_iterator p = _node->parents().begin();
	     p != _node->parents().end(); ++p)
	{
	    out << symtab.getName(*p) << " = ";
	    double const *v = (*p)->value(_chain);
	    vector <unsigned int> dim = drop((*p)->dim());
	    switch(dim.size()) {
	    case 0:
		out << endl;
		break;
	    case 1:
		printVector(out, v, (*p)->length());
		break;
	    case 2:
		printMatrix(out, v, dim[0], dim[1]);
		break;
	    default:
		printArray(out, v, dim);
		break;
	    }
	}
	out << endl;
    }

} //namespace jags
