/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: Gaussian.h
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 22-May-2020 12:13:57
 */

#ifndef GAUSSIAN_H
#define GAUSSIAN_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "svmPredict_types.h"

/* Function Declarations */
extern void Gaussian(const double svT[126], const double svInnerProduct[21],
                     const double x[6], double kernelProduct[21]);

#endif

/*
 * File trailer for Gaussian.h
 *
 * [EOF]
 */
