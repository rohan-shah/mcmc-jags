noinst_LTLIBRARIES = bugsfunc.la

bugsfunc_la_CPPFLAGS = -I$(top_srcdir)/src/include	\
-I$(top_srcdir)/src/modules/bugs/matrix \
-I$(top_srcdir)/src/modules/bugs/distributions

bugsfunc_la_LDFLAGS = -no-undefined -module -avoid-version

bugsfunc_la_SOURCES = Max.cc Probit.cc InProd.cc Mean.cc SD.cc		\
ICLogLog.cc Min.cc Sqrt.cc CLogLog.cc ILogit.cc Step.cc Log.cc Sum.cc	\
Exp.cc LogFact.cc LogGam.cc Phi.cc Logit.cc Inverse.cc LogDet.cc	\
Sort.cc Rank.cc Abs.cc Cos.cc Sin.cc Tan.cc Cosh.cc Sinh.cc Tanh.cc	\
ArcCos.cc ArcSin.cc ArcTan.cc ArcCosh.cc ArcSinh.cc ArcTanh.cc		\
InterpLin.cc Round.cc Trunc.cc Pow.cc Equals.cc Prod.cc MatMult.cc	\
Transpose.cc DSumFunc.cc DIntervalFunc.cc DPQFunction.cc DFunction.cc	\
PFunction.cc QFunction.cc DRoundFunc.cc IfElse.cc

noinst_HEADERS = LogGam.h Phi.h Sum.h Log.h Logit.h Probit.h CLogLog.h	\
InProd.h Max.h SD.h ICLogLog.h Mean.h Sqrt.h ILogit.h Min.h Step.h	\
Exp.h LogFact.h Inverse.h LogDet.h Sort.h Rank.h Abs.h Cos.h Sin.h	\
Tan.h Cosh.h Sinh.h Tanh.h ArcCos.h ArcSin.h ArcTan.h ArcCosh.h		\
ArcSinh.h ArcTanh.h InterpLin.h Round.h Trunc.h Pow.h Equals.h Prod.h	\
MatMult.h Transpose.h DSumFunc.h DIntervalFunc.h \
DPQFunction.h DFunction.h PFunction.h QFunction.h DRoundFunc.h IfElse.h
