/***********************************************************************\
 *
 * File:           RngStream.c for multiple streams of Random Numbers
 * Language:       ANSI C
 * Copyright:      Pierre L'Ecuyer, University of Montreal (lecuyer@iro.UMontreal.ca)
 * Notice:         This code is released under the GPL license
 * Date:           14 August 2001
 *
\***********************************************************************/ 


#include "RngStream.h"

/*---------------------------------------------------------------------*/
/* Private part.                                                       */
/*---------------------------------------------------------------------*/


#define norm  2.328306549295727688e-10
#define m1    4294967087.0
#define m2    4294944443.0
#define a12     1403580.0
#define a13n     810728.0
#define a21      527612.0
#define a23n    1370589.0

#define two17   131072.0
#define two53   9007199254740992.0
#define fact  5.9604644775390625e-8    /* 1 / 2^24 */


/* The following are the transition matrices of the two MRG components */
/* (in matrix form), raised to the powers -1, 1, 2^76, and 2^127, resp.*/
static double InvA1[3][3] = {          /* Inverse of A1p0 */
          { 184888585.0,   0.0,  1945170933.0 },
          {         1.0,   0.0,           0.0 },
          {         0.0,   1.0,           0.0 }
          };

static double InvA2[3][3] = {          /* Inverse of A2p0 */
          {      0.0,  360363334.0,  4225571728.0 },
          {      1.0,          0.0,           0.0 },
          {      0.0,          1.0,           0.0 }
          };

static double A1p0[3][3] = {
          {       0.0,        1.0,       0.0 },
          {       0.0,        0.0,       1.0 },
          { -810728.0,  1403580.0,       0.0 }
          };

static double A2p0[3][3] = {
          {        0.0,        1.0,       0.0 },
          {        0.0,        0.0,       1.0 },
          { -1370589.0,        0.0,  527612.0 }
          };

static double A1p76[3][3] = {
          {      82758667.0, 1871391091.0, 4127413238.0 }, 
          {    3672831523.0,   69195019.0, 1871391091.0 }, 
          {    3672091415.0, 3528743235.0,   69195019.0 }
          };

static double A2p76[3][3] = {
          {    1511326704.0, 3759209742.0, 1610795712.0 }, 
          {    4292754251.0, 1511326704.0, 3889917532.0 }, 
          {    3859662829.0, 4292754251.0, 3708466080.0 }
          };

static double A1p127[3][3] = {
          {    2427906178.0, 3580155704.0,  949770784.0 }, 
          {     226153695.0, 1230515664.0, 3580155704.0 },
          {    1988835001.0,  986791581.0, 1230515664.0 }
          };

static double A2p127[3][3] = {
          {    1464411153.0,  277697599.0, 1610723613.0 },
          {      32183930.0, 1464411153.0, 1022607788.0 },
          {    2824425944.0,   32183930.0, 2093834863.0 }
          };





/*-------------------------------------------------------------------------*/

static double MultModM (double a, double s, double c, double m)
   /* Compute (a*s + c) % m. m must be < 2^35.  Works also for s, c < 0 */
{
   double v;
   int a1;
   v = a * s + c;
   if ((v >= two53) || (v <= -two53)) {
      a1 = (int) (a / two17);
      a -= a1 * two17;
      v = a1 * s;
      a1 = (int) (v / m);
      v -= a1 * m;
      v = v * two17 + a * s + c;
   }
   a1 = (int) (v / m);
   if ((v -= a1 * m) < 0.0)
      return v += m;
   else
      return v;
}


/*-------------------------------------------------------------------------*/

static void MatVecModM (double A[3][3], double s[3], double v[3], double m)
   /* Returns v = A*s % m.  Assumes that -m < s[i] < m. */
   /* Works even if v = s. */
{
   int i;
   double x[3];
   for (i = 0; i < 3; ++i) {
      x[i] = MultModM (A[i][0], s[0], 0.0, m);
      x[i] = MultModM (A[i][1], s[1], x[i], m);
      x[i] = MultModM (A[i][2], s[2], x[i], m);
   }
   for (i = 0; i < 3; ++i)
      v[i] = x[i];
}


/*-------------------------------------------------------------------------*/

static void MatMatModM (double A[3][3], double B[3][3], double C[3][3],
                        double m)
   /* Returns C = A*B % m. Work even if A = C or B = C or A = B = C. */
{
   int i, j;
   double V[3], W[3][3];
   for (i = 0; i < 3; ++i) {
      for (j = 0; j < 3; ++j)
         V[j] = B[j][i];
      MatVecModM (A, V, V, m);
      for (j = 0; j < 3; ++j)
         W[j][i] = V[j];
   }
   for (i = 0; i < 3; ++i) {
      for (j = 0; j < 3; ++j)
         C[i][j] = W[i][j];
   }
}


/*-------------------------------------------------------------------------*/

static void MatTwoPowModM (double A[3][3], double B[3][3], double m, long e)
  /* Compute matrix B = (A^(2^e) % m);  works even if A = B */
{
   long i, j;

   /* initialize: B = A */
   if (A != B) {
      for (i = 0; i < 3; i++) {
         for (j = 0; j < 3; ++j)
            B[i][j] = A[i][j];
      }
   }
   /* Compute B = A^{2^e} */
   for (i = 0; i < e; i++)
      MatMatModM (B, B, B, m);
}


/*-------------------------------------------------------------------------*/

static void MatPowModM (double A[3][3], double B[3][3], double m, long n)
   /* Compute matrix B = A^n % m ;  works even if A = B */
{
   int i, j;
   double W[3][3];

   /* initialize: W = A; B = I */
   for (i = 0; i < 3; i++) {
      for (j = 0; j < 3; ++j) {
         W[i][j] = A[i][j];
         B[i][j] = 0.0;
      }
   }
   for (j = 0; j < 3; ++j)
      B[j][j] = 1.0;

   /* Compute B = A^n % m using the binary decomposition of n */
   while (n > 0) {
      if (n % 2)
         MatMatModM (W, B, B, m);
      MatMatModM (W, W, W, m);
      n /= 2;
   }
}


/*-------------------------------------------------------------------------*/

static bool checkSeed (unsigned int seed[6])
{
    for (int i = 0; i < 3; ++i) {
	if (seed[i] >= m1) 
	    return false;
    }
    for (int i = 3; i < 6; ++i) {
	if (seed[i] >= m2) 
	    return false;
    }
    if (seed[0] == 0 && seed[1] == 0 && seed[2] == 0) {
	return false;
    }
    if (seed[3] == 0 && seed[4] == 0 && seed[5] == 0) {
	return false;
    }

    return true;
}

static void fixSeed (unsigned int seed[6])
{
    for (int i = 0; i < 3; ++i) {
	if (seed[i] >= m1) 
	    seed[i] = 0;
    }
    for (int i = 3; i < 6; ++i) {
	if (seed[i] >= m2) 
	    seed[i] = 0;
    }
    if (seed[0] == 0 && seed[1] == 0 && seed[2] == 0) {
	seed[0] = 1;
    }
    if (seed[3] == 0 && seed[4] == 0 && seed[5] == 0) {
	seed[3] = 1;
    }
}

/*---------------------------------------------------------------------*/
/* Public part.                                                        */
/*---------------------------------------------------------------------*/

namespace lecuyer {

    RngStream::RngStream(unsigned int seed)
	: RmathRNG("lecuyer::RngStream", KINDERMAN_RAMAGE), 
	  Anti(false), IncPrec(false)
    {
	init(seed);
    }

    void RngStream::resetStartStream ()
    {
	for (int i = 0; i < 6; ++i)
	    Cg[i] = Bg[i] = Ig[i];
    }

    void RngStream::resetNextStream ()
    {
	MatVecModM (A1p127, Ig, Ig, m1);
	MatVecModM (A2p127, &Ig[3], &Ig[3], m2);
	resetStartStream();
    }

    void RngStream::resetStartSubstream ()
    {
	for (int i = 0; i < 6; ++i) {
	    Cg[i] = Bg[i];
	}
    }

    void RngStream::resetNextSubstream ()
    {
	MatVecModM (A1p76, Bg, Bg, m1);
	MatVecModM (A2p76, &Bg[3], &Bg[3], m2);
	resetStartSubstream();
    }


    bool RngStream::setState(std::vector<int> const &state)
    {
	/* Pass from int to double via unsigned int */

	if (state.size() != 18)
	    return false;

	unsigned int ig[6], bg[6], cg[6];
	for (int i = 0; i < 6; ++i) {
	    ig[i] = static_cast<unsigned int>(state[3*i]);
	    bg[i] = static_cast<unsigned int>(state[3*i+1]);
	    cg[i] = static_cast<unsigned int>(state[3*i+2]);
	}
    
	if (!checkSeed (ig) || !checkSeed(bg) || !checkSeed(cg))
	    return false;
	
	for (int i = 0; i < 6; ++i) {
	    Ig[i] = ig[i];
	    Bg[i] = bg[i];
	    Cg[i] = cg[i];
	}
	return true;
    }

    void RngStream::advanceState (long e, long c)
    {
	double B1[3][3], C1[3][3], B2[3][3], C2[3][3];

	if (e > 0) {
	    MatTwoPowModM (A1p0, B1, m1, e);
	    MatTwoPowModM (A2p0, B2, m2, e);
	} else if (e < 0) {
	    MatTwoPowModM (InvA1, B1, m1, -e);
	    MatTwoPowModM (InvA2, B2, m2, -e);
	}

	if (c >= 0) {
	    MatPowModM (A1p0, C1, m1, c);
	    MatPowModM (A2p0, C2, m2, c);
	} else {
	    MatPowModM (InvA1, C1, m1, -c);
	    MatPowModM (InvA2, C2, m2, -c);
	}

	if (e) {
	    MatMatModM (B1, C1, C1, m1);
	    MatMatModM (B2, C2, C2, m2);
	}

	MatVecModM (C1, Cg, Cg, m1);
	MatVecModM (C2, &Cg[3], &Cg[3], m2);
    }

    void RngStream::getState(std::vector<int> &state) const
    {
	/* We pass from double to integer via unsigned integer
	   so as not to lose any information */
	   
	state.clear();

	unsigned int ig[6], bg[6], cg[6];
	for (int i = 0; i < 6; ++i) {
	    ig[i] = Ig[i];
	    bg[i] = Bg[i];
	    cg[i] = Cg[i];
	}

	for (int i = 0; i < 6; ++i) {
	    state.push_back(static_cast<int>(ig[i]));
	    state.push_back(static_cast<int>(bg[i]));
	    state.push_back(static_cast<int>(cg[i]));
	}
    }

    void RngStream::increasedPrecis (bool incp)
    {
	IncPrec = incp;
    }

    void RngStream::setAntithetic (bool a)
    {
	Anti = a;
    }

    double RngStream::U01 ()
    {
	/* Component 1 */
	double p1 = a12 * Cg[1] - a13n * Cg[0];
	int k = p1 / m1;
	p1 -= k * m1;
	if (p1 < 0.0)
	    p1 += m1;
	Cg[0] = Cg[1];
	Cg[1] = Cg[2];
	Cg[2] = p1;

	/* Component 2 */
	double p2 = a21 * Cg[5] - a23n * Cg[3];
	k = p2 / m2;
	p2 -= k * m2;
	if (p2 < 0.0)
	    p2 += m2;
	Cg[3] = Cg[4];
	Cg[4] = Cg[5];
	Cg[5] = p2;

	/* Combination */
	double u = ((p1 > p2) ? (p1 - p2) * norm : (p1 - p2 + m1) * norm);
	return (Anti) ? (1 - u) : u;
    }


    double RngStream::U01d ()
    {
	double u = U01();
	if (!Anti) {
	    u += U01() * fact;
	    return (u < 1.0) ? u : (u - 1.0);
	} else {
	    /* Don't forget that U01() returns 1 - u in the antithetic case */
	    u += (U01() - 1.0) * fact;
	    return (u < 0.0) ? u + 1.0 : u;
	}
    }


    double RngStream::uniform()
    {
	return IncPrec ? U01d() : U01();
    }

    void RngStream::init(unsigned int seed)
    {
	/* Initial scrambling */
	for(unsigned int j = 0; j < 50; j++)
	    seed = (69069 * seed + 1);

	unsigned int state[6];
	for (unsigned int j = 0; j < 6; j++) {
	    seed = (69069 * seed + 1);
	    state[j] = seed;
	}

	fixSeed(state);
	for (int i = 0; i < 6; ++i) {
	    Cg[i] = Bg[i] = Ig[i] = state[i];
	}    
    }

}
