/*
 * RC_PID.h
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "RC_PID".
 *
 * Model version              : 1.3
 * Simulink Coder version : 25.2 (R2025b) 28-Jul-2025
 * C source code generated on : Thu Feb 19 02:09:03 2026
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objective: Debugging
 * Validation result: All passed
 */

#ifndef RC_PID_h_
#define RC_PID_h_
#ifndef RC_PID_COMMON_INCLUDES_
#define RC_PID_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#include "rt_nonfinite.h"
#include "math.h"
#endif                                 /* RC_PID_COMMON_INCLUDES_ */

#include "RC_PID_types.h"
#include <float.h>
#include <string.h>
#include <stddef.h>

/* Macros for accessing real-time model data structure */
#ifndef rtmGetFinalTime
#define rtmGetFinalTime(rtm)           ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetRTWLogInfo
#define rtmGetRTWLogInfo(rtm)          ((rtm)->rtwLogInfo)
#endif

#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   ((rtm)->Timing.taskTime0)
#endif

#ifndef rtmGetTFinal
#define rtmGetTFinal(rtm)              ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                (&(rtm)->Timing.taskTime0)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T totalreartorque;              /* '<Root>/Add1' */
  real_T Gain;                         /* '<Root>/Gain' */
  real_T Divide;                       /* '<Root>/Divide' */
  real_T Tan;                          /* '<Root>/Tan' */
  real_T Product;                      /* '<Root>/Product' */
  real_T Add;                          /* '<Root>/Add' */
  real_T ProportionalGain;             /* '<S41>/Proportional Gain' */
  real_T Integrator;                   /* '<S36>/Integrator' */
  real_T DerivativeGain;               /* '<S29>/Derivative Gain' */
  real_T Filter;                       /* '<S31>/Filter' */
  real_T SumD;                         /* '<S31>/SumD' */
  real_T FilterCoefficient;            /* '<S39>/Filter Coefficient' */
  real_T Sum;                          /* '<S45>/Sum' */
  real_T IntegralGain;                 /* '<S33>/Integral Gain' */
} B_RC_PID_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T Integrator_DSTATE;            /* '<S36>/Integrator' */
  real_T Filter_DSTATE;                /* '<S31>/Filter' */
} DW_RC_PID_T;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T LongitudinalVelocity;         /* '<Root>/Longitudinal Velocity' */
  real_T Steeringangle;                /* '<Root>/Steering angle' */
  real_T measureyawrate;               /* '<Root>/measure yaw rate' */
} ExtU_RC_PID_T;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T fronttorque;                  /* '<Root>/front torque' */
  real_T RearRightTorque;              /* '<Root>/Rear Right Torque' */
  real_T RearLeftTorque;               /* '<Root>/Rear Left Torque' */
} ExtY_RC_PID_T;

/* Parameters (default storage) */
struct P_RC_PID_T_ {
  real_T DiscretePIDController_D;     /* Mask Parameter: DiscretePIDController_D
                                       * Referenced by: '<S29>/Derivative Gain'
                                       */
  real_T DiscretePIDController_I;     /* Mask Parameter: DiscretePIDController_I
                                       * Referenced by: '<S33>/Integral Gain'
                                       */
  real_T DiscretePIDController_InitialCo;
                              /* Mask Parameter: DiscretePIDController_InitialCo
                               * Referenced by: '<S31>/Filter'
                               */
  real_T DiscretePIDController_Initial_k;
                              /* Mask Parameter: DiscretePIDController_Initial_k
                               * Referenced by: '<S36>/Integrator'
                               */
  real_T DiscretePIDController_N;     /* Mask Parameter: DiscretePIDController_N
                                       * Referenced by: '<S39>/Filter Coefficient'
                                       */
  real_T DiscretePIDController_P;     /* Mask Parameter: DiscretePIDController_P
                                       * Referenced by: '<S41>/Proportional Gain'
                                       */
  real_T frontsplit_Value;             /* Expression: 0.5
                                        * Referenced by: '<Root>/front split'
                                        */
  real_T TotalTorque_Value;            /* Expression: 1
                                        * Referenced by: '<Root>/Total Torque'
                                        */
  real_T Gain_Gain;                    /* Expression: 0.5
                                        * Referenced by: '<Root>/Gain'
                                        */
  real_T Wheelbase_Value;              /* Expression: 1
                                        * Referenced by: '<Root>/Wheelbase'
                                        */
  real_T Integrator_gainval;           /* Computed Parameter: Integrator_gainval
                                        * Referenced by: '<S36>/Integrator'
                                        */
  real_T Filter_gainval;               /* Computed Parameter: Filter_gainval
                                        * Referenced by: '<S31>/Filter'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_RC_PID_T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    time_T taskTime0;
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    time_T tFinal;
    boolean_T stopRequestedFlag;
  } Timing;
};

/* Block parameters (default storage) */
extern P_RC_PID_T RC_PID_P;

/* Block signals (default storage) */
extern B_RC_PID_T RC_PID_B;

/* Block states (default storage) */
extern DW_RC_PID_T RC_PID_DW;

/* External inputs (root inport signals with default storage) */
extern ExtU_RC_PID_T RC_PID_U;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY_RC_PID_T RC_PID_Y;

/* Model entry point functions */
extern void RC_PID_initialize(void);
extern void RC_PID_step(void);
extern void RC_PID_terminate(void);

/* Real-time Model object */
extern RT_MODEL_RC_PID_T *const RC_PID_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'RC_PID'
 * '<S1>'   : 'RC_PID/Discrete PID Controller'
 * '<S2>'   : 'RC_PID/Discrete PID Controller/Anti-windup'
 * '<S3>'   : 'RC_PID/Discrete PID Controller/D Gain'
 * '<S4>'   : 'RC_PID/Discrete PID Controller/External Derivative'
 * '<S5>'   : 'RC_PID/Discrete PID Controller/Filter'
 * '<S6>'   : 'RC_PID/Discrete PID Controller/Filter ICs'
 * '<S7>'   : 'RC_PID/Discrete PID Controller/I Gain'
 * '<S8>'   : 'RC_PID/Discrete PID Controller/Ideal P Gain'
 * '<S9>'   : 'RC_PID/Discrete PID Controller/Ideal P Gain Fdbk'
 * '<S10>'  : 'RC_PID/Discrete PID Controller/Integrator'
 * '<S11>'  : 'RC_PID/Discrete PID Controller/Integrator ICs'
 * '<S12>'  : 'RC_PID/Discrete PID Controller/N Copy'
 * '<S13>'  : 'RC_PID/Discrete PID Controller/N Gain'
 * '<S14>'  : 'RC_PID/Discrete PID Controller/P Copy'
 * '<S15>'  : 'RC_PID/Discrete PID Controller/Parallel P Gain'
 * '<S16>'  : 'RC_PID/Discrete PID Controller/Reset Signal'
 * '<S17>'  : 'RC_PID/Discrete PID Controller/Saturation'
 * '<S18>'  : 'RC_PID/Discrete PID Controller/Saturation Fdbk'
 * '<S19>'  : 'RC_PID/Discrete PID Controller/Sum'
 * '<S20>'  : 'RC_PID/Discrete PID Controller/Sum Fdbk'
 * '<S21>'  : 'RC_PID/Discrete PID Controller/Tracking Mode'
 * '<S22>'  : 'RC_PID/Discrete PID Controller/Tracking Mode Sum'
 * '<S23>'  : 'RC_PID/Discrete PID Controller/Tsamp - Integral'
 * '<S24>'  : 'RC_PID/Discrete PID Controller/Tsamp - Ngain'
 * '<S25>'  : 'RC_PID/Discrete PID Controller/postSat Signal'
 * '<S26>'  : 'RC_PID/Discrete PID Controller/preInt Signal'
 * '<S27>'  : 'RC_PID/Discrete PID Controller/preSat Signal'
 * '<S28>'  : 'RC_PID/Discrete PID Controller/Anti-windup/Passthrough'
 * '<S29>'  : 'RC_PID/Discrete PID Controller/D Gain/Internal Parameters'
 * '<S30>'  : 'RC_PID/Discrete PID Controller/External Derivative/Error'
 * '<S31>'  : 'RC_PID/Discrete PID Controller/Filter/Disc. Forward Euler Filter'
 * '<S32>'  : 'RC_PID/Discrete PID Controller/Filter ICs/Internal IC - Filter'
 * '<S33>'  : 'RC_PID/Discrete PID Controller/I Gain/Internal Parameters'
 * '<S34>'  : 'RC_PID/Discrete PID Controller/Ideal P Gain/Passthrough'
 * '<S35>'  : 'RC_PID/Discrete PID Controller/Ideal P Gain Fdbk/Disabled'
 * '<S36>'  : 'RC_PID/Discrete PID Controller/Integrator/Discrete'
 * '<S37>'  : 'RC_PID/Discrete PID Controller/Integrator ICs/Internal IC'
 * '<S38>'  : 'RC_PID/Discrete PID Controller/N Copy/Disabled'
 * '<S39>'  : 'RC_PID/Discrete PID Controller/N Gain/Internal Parameters'
 * '<S40>'  : 'RC_PID/Discrete PID Controller/P Copy/Disabled'
 * '<S41>'  : 'RC_PID/Discrete PID Controller/Parallel P Gain/Internal Parameters'
 * '<S42>'  : 'RC_PID/Discrete PID Controller/Reset Signal/Disabled'
 * '<S43>'  : 'RC_PID/Discrete PID Controller/Saturation/Passthrough'
 * '<S44>'  : 'RC_PID/Discrete PID Controller/Saturation Fdbk/Disabled'
 * '<S45>'  : 'RC_PID/Discrete PID Controller/Sum/Sum_PID'
 * '<S46>'  : 'RC_PID/Discrete PID Controller/Sum Fdbk/Disabled'
 * '<S47>'  : 'RC_PID/Discrete PID Controller/Tracking Mode/Disabled'
 * '<S48>'  : 'RC_PID/Discrete PID Controller/Tracking Mode Sum/Passthrough'
 * '<S49>'  : 'RC_PID/Discrete PID Controller/Tsamp - Integral/TsSignalSpecification'
 * '<S50>'  : 'RC_PID/Discrete PID Controller/Tsamp - Ngain/Passthrough'
 * '<S51>'  : 'RC_PID/Discrete PID Controller/postSat Signal/Forward_Path'
 * '<S52>'  : 'RC_PID/Discrete PID Controller/preInt Signal/Internal PreInt'
 * '<S53>'  : 'RC_PID/Discrete PID Controller/preSat Signal/Forward_Path'
 */
#endif                                 /* RC_PID_h_ */
