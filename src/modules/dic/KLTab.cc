#include <config.h>
#include "KLTab.h"
#include "KL.h"
#include "KLNorm.h"
#include "KLPois.h"
#include "KLBin.h"
#include "KLBern.h"

using std::string;
using std::map;

namespace dic {

    KLTab::KLTab() {
	_klmap["dnorm"] = new KLNorm;
	_klmap["dbin"] = new KLBin;
	_klmap["dpois"] = new KLPois;
	_klmap["dbern"] = new KLBern;
    }

    KLTab::~KLTab() {
	map<string, KL *>::const_iterator p = _klmap.begin();
	for (; p != _klmap.end(); ++p) {
	    delete p->second;
	}
    }
    
    KL const *KLTab::find(string const &name) const {
	map<string, KL *>::const_iterator p = _klmap.find(name);
	if (p == _klmap.end()) {
	    return 0;
	}
	else {
	    return p->second;
	}
    }
}
