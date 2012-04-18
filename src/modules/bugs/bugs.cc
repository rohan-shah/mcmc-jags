#include <Module.h>
#include <distribution/DistTab.h>

#include <distributions/DBern.h>
#include <distributions/DBeta.h>
#include <distributions/DBetaBin.h>
#include <distributions/DBin.h>
#include <distributions/DCat.h>
#include <distributions/DChisqr.h>
#include <distributions/DDexp.h>
#include <distributions/DDirch.h>
#include <distributions/DExp.h>
#include <distributions/DF.h>
#include <distributions/DGamma.h>
#include <distributions/DGenGamma.h>
#include <distributions/DInterval.h>
#include <distributions/DLnorm.h>
#include <distributions/DLogis.h>
#include <distributions/DMNorm.h>
#include <distributions/DMT.h>
#include <distributions/DMulti.h>
#include <distributions/DNegBin.h>
#include <distributions/DNChisqr.h>
#include <distributions/DNorm.h>
#include <distributions/DPar.h>
#include <distributions/DPois.h>
#include <distributions/DRound.h>
#include <distributions/DT.h>
#include <distributions/DUnif.h>
#include <distributions/DWeib.h>
#include <distributions/DWish.h>
#include <distributions/DHyper.h>
#include <distributions/DSum.h>

#include <functions/Abs.h>
#include <functions/ArcCos.h>
#include <functions/ArcCosh.h>
#include <functions/ArcSin.h>
#include <functions/ArcSinh.h>
#include <functions/ArcTan.h>
#include <functions/ArcTanh.h>
#include <functions/CLogLog.h>
#include <functions/Cos.h>
#include <functions/Cosh.h>
#include <functions/DIntervalFunc.h>
#include <functions/DRoundFunc.h>
#include <functions/DSumFunc.h>
#include <functions/Exp.h>
#include <functions/Equals.h>
#include <functions/ICLogLog.h>
#include <functions/IfElse.h>
#include <functions/ILogit.h>
#include <functions/InProd.h>
#include <functions/InterpLin.h>
#include <functions/Inverse.h>
#include <functions/LogDet.h>
#include <functions/LogFact.h>
#include <functions/LogGam.h>
#include <functions/Log.h>
#include <functions/Logit.h>
#include <functions/Max.h>
#include <functions/Mean.h>
#include <functions/Min.h>
#include <functions/MatMult.h>
#include <functions/Phi.h>
#include <functions/Pow.h>
#include <functions/Probit.h>
#include <functions/Prod.h>
#include <functions/Rank.h>
#include <functions/Round.h>
#include <functions/SD.h>
#include <functions/Sin.h>
#include <functions/Sinh.h>
#include <functions/Sort.h>
#include <functions/Sqrt.h>
#include <functions/Step.h>
#include <functions/Sum.h>
#include <functions/Tan.h>
#include <functions/Tanh.h>
#include <functions/Transpose.h>
#include <functions/Trunc.h>

#include <functions/DFunction.h>
#include <functions/PFunction.h>
#include <functions/QFunction.h>

#include <samplers/ConjugateFactory.h>
#include <samplers/DSumFactory.h>
#include <samplers/MNormalFactory.h>
#include <samplers/DirichletFactory.h>

using std::vector;

namespace bugs {

    class BUGSModule : public Module {
    public:
	BUGSModule();
	~BUGSModule();
	void Rinsert(RScalarDist *dist);
    };

    BUGSModule::BUGSModule() 
	: Module("bugs")
    {

	//Load distributions
	insert(new DBern);
	Rinsert(new DBeta);
	Rinsert(new DBetaBin);
	Rinsert(new DBin);
	insert(new DCat);
	Rinsert(new DChisqr);
	Rinsert(new DDexp);
	insert(new DDirch);
	Rinsert(new DExp);
	Rinsert(new DF);
	Rinsert(new DGamma);
	Rinsert(new DGenGamma);
	Rinsert(new DLnorm);
	Rinsert(new DLogis);
	insert(new DMNorm);
	insert(new DMT);
	insert(new DMulti);
	Rinsert(new DNChisqr);
	Rinsert(new DNegBin);
	Rinsert(new DNorm);
	Rinsert(new DPar);
	Rinsert(new DPois);
	Rinsert(new DT);
	insert(new DUnif);
	Rinsert(new DWeib);
	insert(new DWish);
	Rinsert(new DHyper);

	//Load functions
	insert(new Abs);
	insert(new ArcCos);
	insert(new ArcCosh);
	insert(new ArcSin);
	insert(new ArcSinh);
	insert(new ArcTan);
	insert(new ArcTanh);
	insert(new Cos);
	insert(new Cosh);
	insert(new CLogLog);
	insert(new Exp);
	insert(new Equals);
	insert(new ICLogLog);
	insert(new IfElse);
	insert(new ILogit);
	insert(new InProd);
	insert(new InterpLin);
	insert(new Inverse);
	insert(new Log);
	insert(new LogDet);
	insert(new LogFact);
	insert(new LogGam);
	insert(new Logit);
	insert(new MatMult);
	insert(new Max);
	insert(new Mean);
	insert(new Min);
	insert(new Phi);
	insert(new Pow);
	insert(new Probit);
	insert(new Prod);
	insert(new Rank);
	insert(new Round);
	insert(new SD);
	insert(new Sin);
	insert(new Sinh);
	insert(new Sort);
	insert(new Sqrt);
	insert(new Step);
	insert(new Sum);
	insert(new Tan);
	insert(new Tanh);
	insert(new Transpose);
	insert(new Trunc);

	//Load observable functions
	insert(new DSum, new DSumFunc);
	insert(new DInterval, new DIntervalFunc);
	insert(new DRound, new DRoundFunc);

	//Load sampler factories
	insert(new MNormalFactory);
	insert(new DirichletFactory);
	insert(new ConjugateFactory);
	//insert(new REFactory);
	insert(new DSumFactory);
    }

    BUGSModule::~BUGSModule() {

	vector<Distribution*> const &dvec = distributions();
	for (unsigned int i = 0; i < dvec.size(); ++i) {
	    delete dvec[i];
	}

	vector<Function*> const &fvec = functions();
	for (unsigned int i = 0; i < fvec.size(); ++i) {
	    delete fvec[i];
	}

	vector<SamplerFactory*> const &svec = samplerFactories();
	for (unsigned int i = 0; i < svec.size(); ++i) {
	    delete svec[i];
	}
    }

    void BUGSModule::Rinsert(RScalarDist *dist)
    {
	insert(dist);    
	insert(new DFunction(dist));
	insert(new PFunction(dist));
	insert(new QFunction(dist));
    }

}

bugs::BUGSModule _bugs_module;
