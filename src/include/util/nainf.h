#ifndef NA_INF_H_
#define NA_INF_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#ifdef MAKE_DLL
  extern __declspec(dllexport) const double JAGS_NA;
  extern __declspec(dllexport) const double JAGS_NAN;
  extern __declspec(dllexport) const double JAGS_POSINF;
  extern __declspec(dllexport) const double JAGS_NEGINF;
#else
  extern __declspec(dllimport) const double JAGS_NA;
  extern __declspec(dllimport) const double JAGS_NAN;
  extern __declspec(dllimport) const double JAGS_POSINF;
  extern __declspec(dllimport) const double JAGS_NEGINF;
#endif
#else
  extern const double JAGS_NA;
  extern const double JAGS_NAN;
  extern const double JAGS_POSINF;
  extern const double JAGS_NEGINF;
#endif

  int jags_finite(double);
  int jags_isnan(double);

#ifdef __cplusplus
}
#endif

#endif
