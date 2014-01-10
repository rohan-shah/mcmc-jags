#include <testlib.h>
#include <cfloat>
#include <cmath>

using std::sqrt;

JAGSFixture::JAGSFixture()
    : tol(sqrt(DBL_EPSILON)), 
      F(1), T(1),
      FF(2), FT(2), TF(2), TT(2),
      FFF(3), FFT(3), FTF(3), FTT(3), TFF(3), TFT(3), TTF(3), TTT(3)      
{
    F[0] = false;
    T[0] = true;

    FF[0] = false; FF[1] = false;
    FT[0] = false; FT[1] = true;
    TF[0] = true; TF[1] = false;
    TT[0] = true; TT[1] = true;

    FFF[0] = false; FFF[1] = false; FFT[2] = false;
    FFT[0] = false; FFT[1] = false; FFT[2] = true;
    FTF[0] = false; FTF[1] = true; FTF[2] = false;
    FTT[0] = false; FTT[1] = true; FTT[2] = true;
    TFF[0] = true; TFF[1] = false; TFT[2] = false;
    TFT[0] = true; TFT[1] = false; TFT[2] = true;
    TTF[0] = true; TTF[1] = true; TTF[2] = false;
    TTT[0] = true; TTT[1] = true; TTT[2] = true;
}
