/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: Linear.c
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 24-May-2020 16:34:15
 */

/* Include Files */
#include "Linear.h"
#include "rt_nonfinite.h"
#include "svmPredict.h"

/* Function Definitions */

/*
 * Arguments    : const double svT[144]
 *                const double x[12]
 *                double kernelProduct[12]
 * Return Type  : void
 */
void Linear(const double svT[144], const double x[12], double kernelProduct[12])
{
  int i;
  double d;
  int i1;
  for (i = 0; i < 12; i++) {
    d = 0.0;
    for (i1 = 0; i1 < 12; i1++) {
      d += x[i1] * svT[i1 + 12 * i];
    }

    kernelProduct[i] = d;
  }
}

/*
 * File trailer for Linear.c
 *
 * [EOF]
 */
