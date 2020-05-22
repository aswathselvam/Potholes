/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: svmPredict.c
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 22-May-2020 12:13:57
 */

/* Include Files */
#include "svmPredict.h"
#include "Gaussian.h"
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
  double Alpha[21];
  double Bias;
  double SupportVectorsT[126];
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
 * Arguments    : const double arr[6]
 * Return Type  : double
 */
double svmPredict(const double arr[6])
{
  double y;
  int i2;
  int ic;
  int i;
  double svInnerProduct[21];
  double dv[21];
  int i1;
  double c;
  int iy;
  static const double dv1[126] = { -0.062088181818181813, 0.45282636363636364,
    9.9827718181818188, 0.26227893166696337, 0.91692538262688228,
    3.9460921368483475, 0.02133, -0.077621818181818181, 9.72413,
    0.16556490926521839, 0.1303092628954812, 0.10328136124199788,
    -0.11799818181818181, 0.63196272727272729, 9.4784318181818161,
    0.25737092678926193, 0.91310881322097548, 2.2185734591706363,
    0.010561818181818183, -0.14124909090909094, 9.7215200000000017,
    0.020312022951041491, 0.11797691659426819, 0.049329784714713686,
    0.2232018181818182, -1.3469081818181816, 9.6216781818181811,
    0.11553961371046571, 0.50293059337881174, 0.33476997657550422,
    -0.28321272727272728, 1.769970909090909, 8.3917618181818181,
    0.21862963088707393, 0.48300015116880757, 0.720468218019618,
    -0.27450727272727271, 1.7998836363636364, 8.3803390909090911,
    0.33242192843105, 0.781085483084569, 1.3204055721213428,
    -0.044581818181818174, 0.36070272727272729, 10.406301818181818,
    0.31218113888632615, 0.70555208252957358, 2.441971341940024,
    1.211328181818182, -0.58598636363636369, 9.4142581818181821,
    0.50398699155470639, 0.35312759895745127, 0.66897356289793974,
    -1.265359090909091, -0.21422818181818182, 9.4348172727272726,
    1.2079188521291861, 0.19303912509220411, 1.4925898961006732,
    -0.64170181818181837, -0.40293181818181806, 10.548568181818183,
    1.0631080012944891, 0.49503074009233372, 3.025649890803026,
    1.7859300000000002, -1.4062936363636362, 8.912309999999998,
    0.80264324246828367, 0.38169554134867034, 1.653949989413223,
    0.26844272727272728, -0.22999909090909093, 9.668007272727273,
    0.87736880597717759, 0.65029230532822, 0.67267202788715541,
    -1.2987499999999998, -1.9657809090909091, 8.9836618181818171,
    0.88754758869595263, 0.47105659868968069, 1.7440663360366668,
    0.021328181818181819, -0.018888181818181814, 9.7122699999999984,
    0.019801636204203839, 0.0098954240113113077, 0.019321854983412056,
    0.11878272727272728, -0.015405454545454545, 9.73837090909091,
    0.52296982857696317, 0.014333906908890096, 0.22278928306606446,
    0.20884181818181816, -0.28557363636363636, 9.7057445454545448,
    0.36919045983931331, 0.49839905578306881, 0.02880046435863028,
    -0.033050909090909082, 0.23148909090909087, 9.72444909090909,
    0.52058403941063236, 0.52229500258866246, 0.039584408661629836,
    -0.99649454545454541, 0.21022181818181815, 9.6893736363636389,
    0.48365164094343016, 0.7785375072636922, 0.031314674283066578,
    1.5089045454545456, -0.73466636363636373, 9.6826890909090917,
    1.2699360467705736, 0.77408885986393994, 0.038182723961117937,
    0.11356363636363637, 0.37158000000000013, 9.7162972727272727,
    0.27200929194690127, 1.1637723733101761, 0.18560308047502377 };

  double d;
  int k;
  static const double dv2[21] = { -1.0196666498023945, -79.6188439366542,
    -0.70429566567397006, -18.097672909799513, -0.60200006020550545,
    -0.75831700941184976, -0.64086762628622262, -0.58768229984727838,
    -1.97513847132381, -0.97472733084333563, -0.97197756688644033,
    -0.94324848628025582, -4.20919443056326, -1.1041890353780353,
    68.019295506772153, 17.037655058505241, 22.409953720822209,
    0.49724084360733312, 2.8699687379443368, 0.69820544705896537,
    0.67550216424584442 };

  boolean_T b[2];
  boolean_T b_tmp;
  boolean_T notNaN;
  boolean_T exitg1;
  double classnum;
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
    cl.Bias = 0.14042784520344445;
    memcpy(&cl.SupportVectorsT[0], &dv1[0], 126U * sizeof(double));
    cl.Scale = 1.0;
    cl.Order = 0.0;
    cl.KernelFunction = gaussian;
    memcpy(&cl.Alpha[0], &dv2[0], 21U * sizeof(double));
    cl.Prior[0] = 0.5;
    cl.Prior[1] = 0.5;
    cl.Cost[0] = 0.0;
    cl.Cost[1] = 1.0;
    cl.Cost[2] = 1.0;
    cl.Cost[3] = 0.0;
    cl_not_empty = true;
  }

  i2 = 1;
  ic = -1;
  for (i = 0; i < 21; i++) {
    i1 = i2 - 1;
    ic++;
    i2 += 6;
    iy = i1;
    c = 0.0;
    for (k = 0; k < 6; k++) {
      c += cl.SupportVectorsT[i1] * cl.SupportVectorsT[iy];
      i1++;
      iy++;
    }

    svInnerProduct[ic] = c;
  }

  Gaussian(cl.SupportVectorsT, svInnerProduct, arr, dv);
  c = 0.0;
  for (i2 = 0; i2 < 21; i2++) {
    c += dv[i2] * cl.Alpha[i2];
  }

  Gaussian(cl.SupportVectorsT, svInnerProduct, arr, dv);
  d = 0.0;
  for (i2 = 0; i2 < 21; i2++) {
    d += dv[i2] * cl.Alpha[i2];
  }

  b[0] = rtIsNaN(-(c + 0.14042784520344445));
  b_tmp = rtIsNaN(d + 0.14042784520344445);
  b[1] = b_tmp;
  notNaN = true;
  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k < 2)) {
    if (!b[k]) {
      notNaN = false;
      exitg1 = true;
    } else {
      k++;
    }
  }

  notNaN = !notNaN;
  if ((cl.Prior[0] < cl.Prior[1]) || (rtIsNaN(cl.Prior[0]) && (!rtIsNaN
        (cl.Prior[1])))) {
    i2 = 1;
  } else {
    i2 = 0;
  }

  classnum = rtNaN;
  if (notNaN) {
    if ((-(c + 0.14042784520344445) < d + 0.14042784520344445) || (rtIsNaN(-(c +
           0.14042784520344445)) && (!b_tmp))) {
      ic = 2;
    } else {
      ic = 1;
    }

    classnum = ic;
  }

  y = cl.ClassNames[i2];
  if (notNaN) {
    if (classnum < 4.294967296E+9) {
      i2 = (int)classnum;
    } else {
      i2 = 0;
    }

    y = cl.ClassNames[i2 - 1];
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
