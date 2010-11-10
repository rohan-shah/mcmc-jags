#ifndef MATRIX_H_
#define MATRIX_H_

bool inverse (double *X, double const *A, int n);
bool inverse_spd (double *X, double const *A, int n);
double logdet(double const *a, int n);

#endif /* MATRIX_H_ */
