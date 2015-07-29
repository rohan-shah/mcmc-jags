dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.63)

AC_INIT([JAGS],[4.0.0beta],[martyn_plummer@users.sourceforge.net],[JAGS])
JAGS_MAJOR=4
JAGS_MINOR=0
AC_SUBST(JAGS_MAJOR)
AC_SUBST(JAGS_MINOR)

AC_CANONICAL_HOST
AC_CONFIG_SRCDIR([src/lib/Console.cc])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects])
AM_PATH_CPPUNIT(1.12.1)

dnl libtool and ltdl configuration
LT_PREREQ(2.2.6)
LT_CONFIG_LTDL_DIR([libltdl])
LT_INIT([dlopen disable-static win32-dll])
LTDL_INIT([recursive])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_F77
AM_PROG_LEX
AC_PROG_YACC

dnl IEEE 754 arithmetic
AC_CHECK_HEADERS(ieeefp.h)
R_IEEE_754
if test "${ac_cv_func_finite}" = "yes"; then
R_FUNC_FINITE
fi
if test "${ac_cv_have_decl_isfinite}" = "yes"; then
R_FUNC_ISFINITE
fi

case "${host_os}" in
  mingw*)
    win=true ;;
  *)
    win=false ;;
esac
AM_CONDITIONAL(WINDOWS, test x$win = xtrue)

dnl fortran stuff
AC_F77_WRAPPERS
AC_F77_LIBRARY_LDFLAGS
ACX_LAPACK([],[
	AC_MSG_ERROR("You need to install the LAPACK library")
])

jagsmoddir=${libdir}/JAGS/modules-${JAGS_MAJOR}
AC_SUBST(jagsmoddir)

AC_CONFIG_FILES([
  Makefile
  m4/Makefile
  etc/Makefile
  etc/jags.pc
  libltdl/Makefile
  src/Makefile
  src/lib/Makefile
  src/lib/version.cc
  src/lib/util/Makefile
  src/lib/sarray/Makefile
  src/lib/function/Makefile
  src/lib/distribution/Makefile
  src/lib/graph/Makefile
  src/lib/sampler/Makefile
  src/lib/model/Makefile
  src/lib/module/Makefile
  src/lib/compiler/Makefile
  src/lib/rng/Makefile
  src/jrmath/Makefile	
  src/include/Makefile
  src/include/util/Makefile
  src/include/sarray/Makefile
  src/include/function/Makefile
  src/include/distribution/Makefile
  src/include/graph/Makefile
  src/include/sampler/Makefile
  src/include/model/Makefile
  src/include/module/Makefile
  src/include/compiler/Makefile
  src/include/rng/Makefile
  src/modules/Makefile
  src/modules/base/Makefile
  src/modules/base/functions/Makefile
  src/modules/base/rngs/Makefile
  src/modules/base/samplers/Makefile
  src/modules/base/monitors/Makefile
  src/modules/bugs/Makefile	
  src/modules/bugs/functions/Makefile	
  src/modules/bugs/distributions/Makefile	
  src/modules/bugs/samplers/Makefile	
  src/modules/bugs/matrix/Makefile	
  src/modules/msm/Makefile	
  src/modules/msm/functions/Makefile	
  src/modules/msm/distributions/Makefile	
  src/modules/msm/matrix/Makefile	
  src/modules/mix/Makefile	
  src/modules/mix/distributions/Makefile	
  src/modules/mix/samplers/Makefile	
  src/modules/dic/Makefile
  src/modules/lecuyer/Makefile
  src/modules/glm/Makefile
  src/modules/glm/CSparse/Makefile
  src/modules/glm/SSparse/Makefile
  src/modules/glm/SSparse/UFconfig/Makefile
  src/modules/glm/SSparse/AMD/Makefile
  src/modules/glm/SSparse/COLAMD/Makefile
  src/modules/glm/SSparse/CHOLMOD/Makefile
  src/modules/glm/SSparse/CHOLMOD/Check/Makefile
  src/modules/glm/SSparse/CHOLMOD/Cholesky/Makefile
  src/modules/glm/SSparse/CHOLMOD/Core/Makefile
  src/modules/glm/SSparse/CHOLMOD/Include/Makefile
  src/modules/glm/SSparse/CHOLMOD/MatrixOps/Makefile
  src/modules/glm/SSparse/CHOLMOD/Modify/Makefile
  src/modules/glm/SSparse/CHOLMOD/Supernodal/Makefile
  src/modules/glm/samplers/Makefile
  src/terminal/Makefile
  win/Makefile
  win/runtime32/Makefile
  win/runtime64/Makefile
  doc/Makefile
  doc/manual/Makefile
  test/Makefile
])
AC_OUTPUT
