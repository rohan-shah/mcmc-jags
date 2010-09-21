#include <compiler/NodeFactory.h>
#include <graph/Node.h>
#include <util/nainf.h>

#include <cfloat>
#include <cmath>

using std::vector;

/* Comparison function for arrays of doubles of equal length */
bool lt(double const *value1, double const *value2, unsigned int length)
{
    for (unsigned long i = 0; i < length; ++i) {
        if (lt(value1[i], value2[i])) {
            return true;
        }
        else if (lt(value2[i], value1[i])) {
            return false;
        }
    }
    return false;
}

/* Comparison function for STL vectors of equal length */
bool lt(vector<double> const &value1, vector<double> const &value2)
{
    for (unsigned long i = 0; i < value1.size(); ++i) {
        if (lt(value1[i], value2[i])) {
            return true;
        }
        else if (lt(value2[i], value1[i])) {
            return false;
        }
    }
    return false;
}

/* Comparison function for Nodes */
bool lt(Node const *node1, Node const *node2)
{
    if (node1 == node2) {
	// A node is always identical to itself
	return false; 
    }

    bool ob1 = node1->isObserved();
    bool ob2 = node2->isObserved();

    if (ob1 && !ob2) {
	//Observed nodes come before unobserved nodes
	return true;
    }
    else if (!ob1 && ob2) {
	return false;
    }
    else if (ob1 && ob2) {
	//Observed nodes are sorted by dimension, then value
	if (node1->dim() == node2->dim()) {
	    return lt(node1->value(0), node2->value(0), node1->length());
	}
	else {
	    return (node1->dim() < node2->dim());
	}
    }
    else {
	//Unobserved nodes are sorted by address. The ordering is
	//arbitrary, but unique.
	return (node1 < node2);
    }
}

/* Comparison operator for vectors of parameters */
bool lt(vector<Node const *> const &par1, vector<Node const *> const &par2)
{
    if (par1.size() == par2.size()) {
        //Equal sized vectors: Sort by ordering of elements 
	for (unsigned int i = 0; i < par1.size(); ++i) {
	    if (lt(par1[i], par2[i])) {
		return true;
	    }
	    else if (lt(par2[i], par1[i])) {
		return false;
	    }
	}
	return false;
    }
    else {
	return par1.size() < par2.size();
    }
}
