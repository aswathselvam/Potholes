/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: svmPredict.c
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 24-May-2020 16:34:15
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
  double Alpha[12];
  double Bias;
  double SupportVectorsT[144];
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
  double dv[12];
  double d;
  int k;
  double d1;
  static const double dv1[144] = { -0.27387000000000006, -1.7943557692307692,
    9.6616778846153846, 0.43223760692833896, 1.0845840886904865,
    0.2686334000837115, -1.4961348076923076, 0.43198769230769229,
    -2.3420030769230769, 3.0217864571199096, 6.5540286610853249,
    7.3305920256909447, 0.18577269230769228, -2.532323653846154,
    9.56308923076923, 0.58564557478994139, 1.183092349455988, 1.9764339064774985,
    -3.0264171153846173, 0.559580192307692, 9.28523942307692, 22.420505086241956,
    11.728948029056813, 6.8609586907908779, -0.11968115384615383,
    -2.8532036538461543, 9.7532732692307675, 0.96848392097488012,
    2.4656198350198375, 2.4038964391739959, -1.1015053846153842,
    -2.7504351923076906, 16.231303653846155, 28.850728947180727,
    21.244136635536659, 9.58199530876059, -0.063759615384615387,
    -1.9564698076923077, 9.2887540384615388, 0.4305287195629075,
    1.1614515733719584, 2.1345698715677917, 0.64584615384615363,
    -1.6506271153846148, -8.2922784615384622, 16.416429102694554,
    19.668141382993792, 12.985392030894152, 0.089922115384615392,
    -3.1356142307692307, 9.3247953846153848, 0.37724376592692888,
    0.45540903244095676, 0.32627213109798658, -0.19697769230769233,
    3.1873988461538452, 6.9041482692307659, 2.0892516386924012,
    5.9706232706636921, 9.13770672241206, 1.9640170588235293, 5.9511676470588224,
    5.8988778431372522, 3.253565145479214, 3.0949725324426312,
    5.1781574550955138, 48.577171764705874, 34.920529803921575,
    14.813482156862747, 143.44786085315735, 70.306792887148362,
    73.264465938855878, 1.2269682352941176, 0.16461784313725553,
    9.0626960784313724, 2.8788150573732283, 4.1423895749310278,
    5.1494620241237161, -56.349397647058844, -1.7798984313725492,
    -16.175722941176478, 114.77279999610248, 72.296831470457192,
    51.409223489950215, -0.25553921568627447, -1.9193225490196073,
    9.62816392156863, 0.35928028211324442, 0.50478060937339164,
    0.38351608517024904, -0.37745901960784317, -3.2275427450980385,
    2.9987941176470589, 1.7825176630555502, 8.37515862761299, 10.382845098973435,
    0.13117156862745097, -1.9391578431372547, 9.62383450980392,
    1.1873334671428619, 0.60176275642918853, 0.45183542042347125,
    -0.49406196078431364, 2.236196078431373, -0.23632568627451142,
    4.5098689077426712, 21.948568733526667, 25.463797130883549,
    -0.42464470588235287, -1.9062674509803912, 9.6146280392156829,
    0.52142280641089311, 0.55213898009411766, 0.18554192274544967,
    -0.6849774509803922, -0.77890078431372545, 2.5801078431372555,
    1.4899964220733461, 4.958374774517087, 6.0811298493054124,
    -0.35969960784313731, -1.8889192156862742, 9.62838823529412,
    0.16942434858025315, 0.46727971395661133, 0.2349953951864239,
    -0.15661372549019614, -0.26695882352941169, 0.083741568627450969,
    1.1173751802487126, 4.6965801746831275, 0.45650382777528858,
    0.31474058823529405, -1.8317617647058824, 9.6043074509803912,
    0.44012994105110259, 0.57477950862119609, 0.5810484914801628,
    -0.38033941176470581, 0.64428215686274515, 0.11800862745098036,
    1.6928577102502285, 5.5237768611041185, 0.946690106573465 };

  boolean_T b[2];
  boolean_T b_tmp;
  boolean_T notNaN;
  static const double dv2[12] = { -1.0, -0.35280544542526515,
    -0.10612579692028096, -0.34619829610273622, -0.44000507859030241,
    0.057420012545834009, 0.0759264026380932, 0.2096432579719347,
    0.081114479569773482, 1.0, 0.036517049672048545, 0.78451341464089941 };

  boolean_T exitg1;
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
    cl.Bias = 4.1650785263848169;
    memcpy(&cl.SupportVectorsT[0], &dv1[0], 144U * sizeof(double));
    cl.Scale = 1.0;
    cl.Order = 0.0;
    cl.KernelFunction = linear;
    memcpy(&cl.Alpha[0], &dv2[0], 12U * sizeof(double));
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
  for (k = 0; k < 12; k++) {
    d += dv[k] * cl.Alpha[k];
  }

  Linear(cl.SupportVectorsT, arr, dv);
  d1 = 0.0;
  for (k = 0; k < 12; k++) {
    d1 += dv[k] * cl.Alpha[k];
  }

  b[0] = rtIsNaN(-(d + 4.1650785263848169));
  b_tmp = rtIsNaN(d1 + 4.1650785263848169);
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
    k = 1;
  } else {
    k = 0;
  }

  classnum = rtNaN;
  if (notNaN) {
    if ((-(d + 4.1650785263848169) < d1 + 4.1650785263848169) || (rtIsNaN(-(d +
           4.1650785263848169)) && (!b_tmp))) {
      iidx = 2;
    } else {
      iidx = 1;
    }

    classnum = iidx;
  }

  y = cl.ClassNames[k];
  if (notNaN) {
    if (classnum < 4.294967296E+9) {
      k = (int)classnum;
    } else {
      k = 0;
    }

    y = cl.ClassNames[k - 1];
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
