#ifndef NA_INF_H_
#define NA_INF_H_

extern "C" {

  extern const double JAGS_NA;
  extern const double JAGS_POSINF;
  extern const double JAGS_NEGINF;

  bool jags_finite(double);
  bool jags_isnan(double);

}

#endif
