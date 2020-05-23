/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: Gaussian.c
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 23-May-2020 16:32:18
 */

/* Include Files */
#include "Gaussian.h"
#include "rt_nonfinite.h"
#include "svmPredict.h"
#include <math.h>

/* Function Definitions */

/*
 * Arguments    : const double svT[5448]
 *                const double svInnerProduct[454]
 *                const double x[12]
 *                double kernelProduct[454]
 * Return Type  : void
 */
void Gaussian(const double svT[5448], const double svInnerProduct[454], const
              double x[12], double kernelProduct[454])
{
  int k;
  double b_x;
  int i;
  double y[454];
  for (k = 0; k < 454; k++) {
    b_x = 0.0;
    for (i = 0; i < 12; i++) {
      b_x += -2.0 * x[i] * svT[i + 12 * k];
    }

    y[k] = b_x;
  }

  b_x = 0.0;
  for (k = 0; k < 12; k++) {
    b_x += x[k] * x[k];
  }

  for (k = 0; k < 454; k++) {
    kernelProduct[k] = exp(-((y[k] + b_x) + svInnerProduct[k]));
  }
}

/*
 * File trailer for Gaussian.c
 *
 * [EOF]
 */
