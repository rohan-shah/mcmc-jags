#include <Module.h>

#include "Add.h"
#include "And.h"
#include "Divide.h"
#include "Equal.h"
#include "GreaterOrEqual.h"
#include "GreaterThan.h"
#include "LessOrEqual.h"
#include "LessThan.h"
#include "Multiply.h"
#include "Neg.h"
#include "Not.h"
#include "NotEqual.h"
#include "Or.h"
#include "Pow.h"
#include "Subtract.h"

using std::vector;

namespace basefunctions {

class BaseFuncModule : public Module {
public:
    BaseFuncModule();
    ~BaseFuncModule();
};

BaseFuncModule::BaseFuncModule() {

    insert(new Add);
    insert(new And);
    insert(new Divide);
    insert(new Equal);
    insert(new GreaterOrEqual);
    insert(new GreaterThan);
    insert(new LessOrEqual);
    insert(new LessThan);
    insert(new Multiply);
    insert(new Neg);
    insert(new Not);
    insert(new NotEqual);
    insert(new Or);
    insert(new Pow);
    insert(new Subtract);
}

BaseFuncModule::~BaseFuncModule() {

    vector<Function*> const &fvec = functions();
    for (unsigned int i = 0; i < fvec.size(); ++i) {
	delete fvec[i];
    }
}

}

basefunctions::BaseFuncModule _base_func_module;

