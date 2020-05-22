/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: svmPredict_terminate.c
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 22-May-2020 12:13:57
 */

/* Include Files */
#include "svmPredict_terminate.h"
#include "rt_nonfinite.h"
#include "svmPredict.h"
#include "svmPredict_data.h"

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void svmPredict_terminate(void)
{
  /* (no terminate code required) */
  isInitialized_svmPredict = false;
}

/*
 * File trailer for svmPredict_terminate.c
 *
 * [EOF]
 */
