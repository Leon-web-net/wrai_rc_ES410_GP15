/*
 * RC_PID.c
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

#include "RC_PID.h"
#include <math.h>
#include <string.h>
#include "RC_PID_private.h"

/* Block signals (default storage) */
B_RC_PID_T RC_PID_B;

/* Block states (default storage) */
DW_RC_PID_T RC_PID_DW;

/* External inputs (root inport signals with default storage) */
ExtU_RC_PID_T RC_PID_U;

/* External outputs (root outports fed by signals with default storage) */
ExtY_RC_PID_T RC_PID_Y;

/* Real-time model */
static RT_MODEL_RC_PID_T RC_PID_M_;
RT_MODEL_RC_PID_T *const RC_PID_M = &RC_PID_M_;

/* Model step function */
void RC_PID_step(void)
{
  /* Outport: '<Root>/front torque' incorporates:
   *  Constant: '<Root>/front split'
   */
  RC_PID_Y.fronttorque = RC_PID_P.frontsplit_Value;

  /* Sum: '<Root>/Add1' incorporates:
   *  Constant: '<Root>/Total Torque'
   *  Constant: '<Root>/front split'
   */
  RC_PID_B.totalreartorque = RC_PID_P.TotalTorque_Value -
    RC_PID_P.frontsplit_Value;

  /* Gain: '<Root>/Gain' */
  RC_PID_B.Gain = RC_PID_P.Gain_Gain * RC_PID_B.totalreartorque;

  /* Product: '<Root>/Divide' incorporates:
   *  Constant: '<Root>/Wheelbase'
   *  Inport: '<Root>/Longitudinal Velocity'
   */
  RC_PID_B.Divide = RC_PID_U.LongitudinalVelocity / RC_PID_P.Wheelbase_Value;

  /* Trigonometry: '<Root>/Tan' incorporates:
   *  Inport: '<Root>/Steering angle'
   */
  RC_PID_B.Tan = tan(RC_PID_U.Steeringangle);

  /* Product: '<Root>/Product' */
  RC_PID_B.Product = RC_PID_B.Divide * RC_PID_B.Tan;

  /* Sum: '<Root>/Add' incorporates:
   *  Inport: '<Root>/measure yaw rate'
   */
  RC_PID_B.Add = RC_PID_B.Product - RC_PID_U.measureyawrate;

  /* Gain: '<S41>/Proportional Gain' */
  RC_PID_B.ProportionalGain = RC_PID_P.DiscretePIDController_P * RC_PID_B.Add;

  /* DiscreteIntegrator: '<S36>/Integrator' */
  RC_PID_B.Integrator = RC_PID_DW.Integrator_DSTATE;

  /* Gain: '<S29>/Derivative Gain' */
  RC_PID_B.DerivativeGain = RC_PID_P.DiscretePIDController_D * RC_PID_B.Add;

  /* DiscreteIntegrator: '<S31>/Filter' */
  RC_PID_B.Filter = RC_PID_DW.Filter_DSTATE;

  /* Sum: '<S31>/SumD' */
  RC_PID_B.SumD = RC_PID_B.DerivativeGain - RC_PID_B.Filter;

  /* Gain: '<S39>/Filter Coefficient' */
  RC_PID_B.FilterCoefficient = RC_PID_P.DiscretePIDController_N * RC_PID_B.SumD;

  /* Sum: '<S45>/Sum' */
  RC_PID_B.Sum = (RC_PID_B.ProportionalGain + RC_PID_B.Integrator) +
    RC_PID_B.FilterCoefficient;

  /* Outport: '<Root>/Rear Right Torque' incorporates:
   *  Sum: '<Root>/add rr'
   */
  RC_PID_Y.RearRightTorque = RC_PID_B.Gain + RC_PID_B.Sum;

  /* Outport: '<Root>/Rear Left Torque' incorporates:
   *  Sum: '<Root>/Add rl'
   */
  RC_PID_Y.RearLeftTorque = RC_PID_B.Gain - RC_PID_B.Sum;

  /* Gain: '<S33>/Integral Gain' */
  RC_PID_B.IntegralGain = RC_PID_P.DiscretePIDController_I * RC_PID_B.Add;

  /* Update for DiscreteIntegrator: '<S36>/Integrator' */
  RC_PID_DW.Integrator_DSTATE += RC_PID_P.Integrator_gainval *
    RC_PID_B.IntegralGain;

  /* Update for DiscreteIntegrator: '<S31>/Filter' */
  RC_PID_DW.Filter_DSTATE += RC_PID_P.Filter_gainval *
    RC_PID_B.FilterCoefficient;

  /* Matfile logging */
  rt_UpdateTXYLogVars(RC_PID_M->rtwLogInfo, (&RC_PID_M->Timing.taskTime0));

  /* signal main to stop simulation */
  {                                    /* Sample time: [0.05s, 0.0s] */
    if ((rtmGetTFinal(RC_PID_M)!=-1) &&
        !((rtmGetTFinal(RC_PID_M)-RC_PID_M->Timing.taskTime0) >
          RC_PID_M->Timing.taskTime0 * (DBL_EPSILON))) {
      rtmSetErrorStatus(RC_PID_M, "Simulation finished");
    }
  }

  /* Update absolute time for base rate */
  /* The "clockTick0" counts the number of times the code of this task has
   * been executed. The absolute time is the multiplication of "clockTick0"
   * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
   * overflow during the application lifespan selected.
   * Timer of this task consists of two 32 bit unsigned integers.
   * The two integers represent the low bits Timing.clockTick0 and the high bits
   * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
   */
  if (!(++RC_PID_M->Timing.clockTick0)) {
    ++RC_PID_M->Timing.clockTickH0;
  }

  RC_PID_M->Timing.taskTime0 = RC_PID_M->Timing.clockTick0 *
    RC_PID_M->Timing.stepSize0 + RC_PID_M->Timing.clockTickH0 *
    RC_PID_M->Timing.stepSize0 * 4294967296.0;
}

/* Model initialize function */
void RC_PID_initialize(void)
{
  /* Registration code */

  /* initialize real-time model */
  (void) memset((void *)RC_PID_M, 0,
                sizeof(RT_MODEL_RC_PID_T));
  rtmSetTFinal(RC_PID_M, 10.0);
  RC_PID_M->Timing.stepSize0 = 0.05;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    RC_PID_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(RC_PID_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(RC_PID_M->rtwLogInfo, (NULL));
    rtliSetLogT(RC_PID_M->rtwLogInfo, "tout");
    rtliSetLogX(RC_PID_M->rtwLogInfo, "");
    rtliSetLogXFinal(RC_PID_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(RC_PID_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(RC_PID_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(RC_PID_M->rtwLogInfo, 0);
    rtliSetLogDecimation(RC_PID_M->rtwLogInfo, 1);
    rtliSetLogY(RC_PID_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(RC_PID_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(RC_PID_M->rtwLogInfo, (NULL));
  }

  /* block I/O */
  (void) memset(((void *) &RC_PID_B), 0,
                sizeof(B_RC_PID_T));

  /* states (dwork) */
  (void) memset((void *)&RC_PID_DW, 0,
                sizeof(DW_RC_PID_T));

  /* external inputs */
  (void)memset(&RC_PID_U, 0, sizeof(ExtU_RC_PID_T));

  /* external outputs */
  (void)memset(&RC_PID_Y, 0, sizeof(ExtY_RC_PID_T));

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(RC_PID_M->rtwLogInfo, 0.0, rtmGetTFinal
    (RC_PID_M), RC_PID_M->Timing.stepSize0, (&rtmGetErrorStatus(RC_PID_M)));

  /* InitializeConditions for DiscreteIntegrator: '<S36>/Integrator' */
  RC_PID_DW.Integrator_DSTATE = RC_PID_P.DiscretePIDController_Initial_k;

  /* InitializeConditions for DiscreteIntegrator: '<S31>/Filter' */
  RC_PID_DW.Filter_DSTATE = RC_PID_P.DiscretePIDController_InitialCo;
}

/* Model terminate function */
void RC_PID_terminate(void)
{
  /* (no terminate code required) */
}
