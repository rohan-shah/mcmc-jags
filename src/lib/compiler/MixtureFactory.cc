#include <config.h>
#include <compiler/MixtureFactory.h>
#include <compiler/NodeFactory.h>
#include <graph/MixtureNode.h>
#include <model/NodeArray.h>

#include <utility>

using std::vector;
using std::map;

bool compMixPair(MixPair const &arg1, MixPair const &arg2)
{
    //First compare indices
    vector<Node const *> const &index1 = arg1.first;
    vector<Node const *> const &index2 = arg2.first;
    if (lt(index1, index2)) {
	return true;
    }
    else if (lt(index2, index1)) {
	return false;
    }

    //Same indices. Now compare mixmaps 
    MixMap const &mixmap1 = arg1.second;
    MixMap const &mixmap2 = arg2.second;
    if (mixmap1.size() < mixmap2.size()) {
	return true;
    }
    else if (mixmap1.size() > mixmap2.size()) {
	return false;
    }
    else {
	MixMap::const_iterator p1 = mixmap1.begin();
	MixMap::const_iterator p2 = mixmap2.begin();
	for (unsigned int i = 0; i < mixmap1.size(); ++i) {
	    if (p1->first < p2->first) {
		return true;
	    }
	    else if (p2->first < p1->first) {
		return false;
	    }
	    else if (lt(p1->second, p2->second)) {
		return true;
	    }
	    else if (lt(p2->second, p1->second)) {
		return false;
	    }
	    ++p1;
	    ++p2;
	}
	return false; //equal
    }
}

MixtureNode * MixtureFactory::getMixtureNode(vector<Node const *> const &index,
					     MixMap const &mixmap, Graph &graph)
{
    MixPair mpair(index, mixmap);
    map<MixPair, MixtureNode*, ltmixpair>::const_iterator p = 
	_mixmap.find(mpair);

    if (p != _mixmap.end()) {
	return p->second;
    }
    else {
	MixtureNode *mix = new MixtureNode(index, mixmap);
	_mixmap[mpair] = mix;
	graph.add(mix);
	return mix;
    }
}
