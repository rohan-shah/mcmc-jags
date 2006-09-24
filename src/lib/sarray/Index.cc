#include <sarray/Index.h>

using std::vector;

Index::Index()
    : vector<int>()
{}

Index::Index(vector<int>::size_type size, int t)
    : vector<int>(size, t)
{}
