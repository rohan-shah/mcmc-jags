#ifndef NA_INF_H_
#define NA_INF_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MAKE_DLL
  extern __declspec(dllexport) const double JAGS_NA;
#else
  extern __declspec(dllimport) const double JAGS_NA;
#endif
  extern const double JAGS_NAN;
  extern const double JAGS_POSINF;
  extern const double JAGS_NEGINF;

  int jags_finite(double);
  int jags_isnan(double);

#ifdef __cplusplus
}
#endif

#endif
