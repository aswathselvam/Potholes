/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: svmPredict.c
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 23-May-2020 23:03:15
 */

/* Include Files */
#include "svmPredict.h"
#include "Linear.h"
#include "rt_nonfinite.h"
#include "svmPredict_data.h"
#include "svmPredict_initialize.h"
#include <string.h>

/* Type Definitions */
#ifndef enum_c_classreg_learning_coderutils_
#define enum_c_classreg_learning_coderutils_

enum c_classreg_learning_coderutils_
{
  Logit = 0,                           /* Default value */
  Doublelogit,
  Invlogit,
  Ismax,
  Sign,
  Symmetric,
  Symmetricismax,
  Symmetriclogit,
  Identity
};

#endif                                 /*enum_c_classreg_learning_coderutils_*/

#ifndef typedef_c_classreg_learning_coderutils_
#define typedef_c_classreg_learning_coderutils_

typedef enum c_classreg_learning_coderutils_ c_classreg_learning_coderutils_;

#endif                                 /*typedef_c_classreg_learning_coderutils_*/

#ifndef enum_d_classreg_learning_coderutils_
#define enum_d_classreg_learning_coderutils_

enum d_classreg_learning_coderutils_
{
  linear = 1,                          /* Default value */
  gaussian = 2,
  rbf = 2,
  polynomial = 3
};

#endif                                 /*enum_d_classreg_learning_coderutils_*/

#ifndef typedef_d_classreg_learning_coderutils_
#define typedef_d_classreg_learning_coderutils_

typedef enum d_classreg_learning_coderutils_ d_classreg_learning_coderutils_;

#endif                                 /*typedef_d_classreg_learning_coderutils_*/

#ifndef typedef_c_classreg_learning_coder_class
#define typedef_c_classreg_learning_coder_class

typedef struct {
  double Alpha[7];
  double Bias;
  double SupportVectorsT[84];
  double Scale;
  double Order;
  d_classreg_learning_coderutils_ KernelFunction;
  double ClassNames[2];
  int ClassNamesLength[2];
  c_classreg_learning_coderutils_ ScoreTransform;
  double Prior[2];
  boolean_T ClassLogicalIndices[2];
  double Cost[4];
} c_classreg_learning_coder_class;

#endif                                 /*typedef_c_classreg_learning_coder_class*/

/* Variable Definitions */
static c_classreg_learning_coder_class cl;
static boolean_T cl_not_empty;

/* Function Definitions */

/*
 * #coder
 * Arguments    : const double arr[12]
 * Return Type  : double
 */
double svmPredict(const double arr[12])
{
  double y;
  double dv[7];
  double d;
  int i;
  double d1;
  static const double dv1[84] = { -0.034310909090909086, -0.97176545454545438,
    9.552867272727271, 0.58652938104505115, 0.545184159571124,
    0.41363940963334034, -12.172937272727273, 0.6881245454545456,
    -6.4449154545454546, 12.391810575917541, 2.334953519564634,
    9.2392190553437619, -0.2020790909090909, -1.144970909090909,
    10.009513636363637, 0.17457099240449689, 0.1622668526504748,
    0.36012615057151093, -11.651159090909088, 1.4260854545454551,
    -1.0294854545454546, 24.486411718844987, 3.6467592853912465,
    2.447075128288315, -0.3617472727272727, -2.641681818181818,
    9.8008527272727264, 0.42006152842389438, 0.69589070539587161,
    0.76292505949262035, 0.36957545454545482, -1.1957536363636365,
    2.9474627272727272, 15.600367597255115, 6.3576590548774679,
    2.5189903284018018, -0.19636818181818178, -1.3749554545454545,
    9.0222063636363625, 0.23771236416384325, 0.92320760320053286,
    2.8064624862850835, 46.715433636363628, -4.052770909090909,
    4.4808418181818181, 52.444910130766218, 7.6343053575743927,
    4.2365884701462759, -0.16972090909090906, -1.11441, 9.7016018181818175,
    0.444613639499612, 1.7623569939203576, 2.82557306491203, 52.270827272727281,
    -7.1202463636363635, 9.953339999999999, 51.137699160740524,
    7.292129272122474, 5.4751061043526832, -0.3364027272727273,
    -0.49042363636363628, 9.7587045454545454, 0.44531089807214264,
    0.61886981339006553, 0.019024943817860029, -0.21634090909090906, -0.9601,
    0.52304727272727292, 0.58156602635392218, 1.0602052803679105,
    15.818281825025176, 0.014255454545454543, -0.43511909090909096,
    9.3186445454545446, 0.1334440917660753, 0.62066601612227079,
    1.8515467779095598, 8.9027909090909088, -0.81377999999999984,
    0.8984245454545452, 31.8989995729228, 4.0675963431638591, 1.8841316302602831
  };

  boolean_T b[2];
  boolean_T b_tmp;
  boolean_T notNaN;
  boolean_T exitg1;
  static const double dv2[7] = { -0.1619087820004019, -0.13811638148364241,
    -0.10274200141187789, -0.019645839917843747, -0.12180877153431979,
    0.10441787375160368, 0.43980390259648106 };

  double classnum;
  int iidx;
  if (isInitialized_svmPredict == false) {
    svmPredict_initialize();
  }

  if (!cl_not_empty) {
    cl.ClassNames[0] = -1.0;
    cl.ClassNamesLength[0] = 1;
    cl.ClassLogicalIndices[0] = true;
    cl.ClassNames[1] = 1.0;
    cl.ClassNamesLength[1] = 1;
    cl.ClassLogicalIndices[1] = true;
    cl.ScoreTransform = Identity;
    cl.Bias = 9.9490748738545047;
    memcpy(&cl.SupportVectorsT[0], &dv1[0], 84U * sizeof(double));
    cl.Scale = 1.0;
    cl.Order = 0.0;
    cl.KernelFunction = linear;
    for (i = 0; i < 7; i++) {
      cl.Alpha[i] = dv2[i];
    }

    cl.Prior[0] = 0.5;
    cl.Prior[1] = 0.5;
    cl.Cost[0] = 0.0;
    cl.Cost[1] = 1.0;
    cl.Cost[2] = 1.0;
    cl.Cost[3] = 0.0;
    cl_not_empty = true;
  }

  Linear(cl.SupportVectorsT, arr, dv);
  d = 0.0;
  for (i = 0; i < 7; i++) {
    d += dv[i] * cl.Alpha[i];
  }

  Linear(cl.SupportVectorsT, arr, dv);
  d1 = 0.0;
  for (i = 0; i < 7; i++) {
    d1 += dv[i] * cl.Alpha[i];
  }

  b[0] = rtIsNaN(-(d + 9.9490748738545047));
  b_tmp = rtIsNaN(d1 + 9.9490748738545047);
  b[1] = b_tmp;
  notNaN = true;
  i = 0;
  exitg1 = false;
  while ((!exitg1) && (i < 2)) {
    if (!b[i]) {
      notNaN = false;
      exitg1 = true;
    } else {
      i++;
    }
  }

  notNaN = !notNaN;
  if ((cl.Prior[0] < cl.Prior[1]) || (rtIsNaN(cl.Prior[0]) && (!rtIsNaN
        (cl.Prior[1])))) {
    i = 1;
  } else {
    i = 0;
  }

  classnum = rtNaN;
  if (notNaN) {
    if ((-(d + 9.9490748738545047) < d1 + 9.9490748738545047) || (rtIsNaN(-(d +
           9.9490748738545047)) && (!b_tmp))) {
      iidx = 2;
    } else {
      iidx = 1;
    }

    classnum = iidx;
  }

  y = cl.ClassNames[i];
  if (notNaN) {
    if (classnum < 4.294967296E+9) {
      i = (int)classnum;
    } else {
      i = 0;
    }

    y = cl.ClassNames[i - 1];
  }

  return y;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void svmPredict_init(void)
{
  cl_not_empty = false;
}

/*
 * File trailer for svmPredict.c
 *
 * [EOF]
 */
