lib_LTLIBRARIES = libjrmath.la

libjrmath_la_CPPFLAGS = -I$(top_srcdir)/src/include -DMATHLIB_STANDALONE

libjrmath_la_CFLAGS = -std=c99

libjrmath_la_LDFLAGS = -no-undefined -version-info 0:0:0

libjrmath_la_LIBADD = -lm

libjrmath_la_SOURCES = callbacks.cc \
beta.c gamma.c gammalims.c lgamma.c polygamma.c toms708.c \
mlutils.c stirlerr.c bd0.c choose.c \
d1mach.c i1mach.c fsign.c imax2.c imin2.c \
fmin2.c fmax2.c fprec.c fround.c ftrunc.c \
chebyshev.c lgammacor.c lbeta.c \
rbeta.c dbeta.c pbeta.c qbeta.c \
rbinom.c dbinom.c pbinom.c qbinom.c \
rchisq.c dchisq.c pchisq.c qchisq.c \
rexp.c dexp.c pexp.c qexp.c \
rgamma.c dgamma.c pgamma.c qgamma.c \
rhyper.c dhyper.c phyper.c qhyper.c \
rlnorm.c dlnorm.c plnorm.c qlnorm.c \
rlogis.c dlogis.c plogis.c qlogis.c \
rnbinom.c dnbinom.c pnbinom.c qnbinom.c \
rnorm.c dnorm.c pnorm.c qnorm.c \
rpois.c dpois.c ppois.c qpois.c \
rt.c dt.c pt.c qt.c \
runif.c dunif.c punif.c qunif.c \
rweibull.c dweibull.c pweibull.c qweibull.c \
rnchisq.c dnchisq.c pnchisq.c qnchisq.c \
rcauchy.c dcauchy.c pcauchy.c qcauchy.c \
rf.c df.c pf.c qf.c \
cospi.c gamma_cody.c bessel_i.c bessel_j.c bessel_k.c bessel_y.c \
dgeom.c pgeom.c qgeom.c rgeom.c \
dnbeta.c pnbeta.c qnbeta.c \
dnf.c pnf.c qnf.c \
dnt.c pnt.c qnt.c \
sign.c \
rweibull2.c dweibull2.c pweibull2.c qweibull2.c 

noinst_HEADERS = nmath.h dpq.h bessel.h

EXTRA_DIST = README
