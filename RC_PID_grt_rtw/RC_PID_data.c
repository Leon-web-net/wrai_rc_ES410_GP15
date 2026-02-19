/*
 * RC_PID_data.c
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

/* Block parameters (default storage) */
P_RC_PID_T RC_PID_P = {
  /* Mask Parameter: DiscretePIDController_D
   * Referenced by: '<S29>/Derivative Gain'
   */
  0.0,

  /* Mask Parameter: DiscretePIDController_I
   * Referenced by: '<S33>/Integral Gain'
   */
  1.0,

  /* Mask Parameter: DiscretePIDController_InitialCo
   * Referenced by: '<S31>/Filter'
   */
  0.0,

  /* Mask Parameter: DiscretePIDController_Initial_k
   * Referenced by: '<S36>/Integrator'
   */
  0.0,

  /* Mask Parameter: DiscretePIDController_N
   * Referenced by: '<S39>/Filter Coefficient'
   */
  100.0,

  /* Mask Parameter: DiscretePIDController_P
   * Referenced by: '<S41>/Proportional Gain'
   */
  1.0,

  /* Expression: 0.5
   * Referenced by: '<Root>/front split'
   */
  0.5,

  /* Expression: 1
   * Referenced by: '<Root>/Total Torque'
   */
  1.0,

  /* Expression: 0.5
   * Referenced by: '<Root>/Gain'
   */
  0.5,

  /* Expression: 1
   * Referenced by: '<Root>/Wheelbase'
   */
  1.0,

  /* Computed Parameter: Integrator_gainval
   * Referenced by: '<S36>/Integrator'
   */
  0.05,

  /* Computed Parameter: Filter_gainval
   * Referenced by: '<S31>/Filter'
   */
  0.05
};
