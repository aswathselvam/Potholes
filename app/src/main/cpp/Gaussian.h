/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: Gaussian.h
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 23-May-2020 16:32:18
 */

#ifndef GAUSSIAN_H
#define GAUSSIAN_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "svmPredict_types.h"

/* Function Declarations */
extern void Gaussian(const double svT[5448], const double svInnerProduct[454],
                     const double x[12], double kernelProduct[454]);

#endif

/*
 * File trailer for Gaussian.h
 *
 * [EOF]
 */
