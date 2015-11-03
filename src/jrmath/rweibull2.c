/*
 *  Mathlib : A C Library of Special Functions
 *  Copyright (C) 1998 Ross Ihaka
 *  Copyright (C) 2000 The R Core Team
 *  Copyright (C) 2015 Martyn Plummer
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, a copy is available at
 *  http://www.r-project.org/Licenses/
 *
 *  DESCRIPTION
 *
 *    Random variates from the Weibull distribution using the BUGS
 *    parameterization, i.e. x^shape has exponential distribution with
 *    mean 1/rate.
 */

#include "nmath.h"

double rweibull2(double shape, double rate, JRNG *rng)
{
    if (!R_FINITE(shape) || !R_FINITE(rate) || shape <= 0. || rate <= 0.) {
	ML_ERR_return_NAN;
    }

    return pow(-log(unif_rand(rng)) / rate, 1.0 / shape);
}
