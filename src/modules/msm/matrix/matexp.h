#ifndef MATEXP_H_
#define MATEXP_H_

namespace msm {

void MatrixExp(double *expmat, double const *mat, int n, double t);
void MatrixExpPade(double *expmat, double const *mat, int n, double t);

}

#endif /* MATEXP_H_ */
