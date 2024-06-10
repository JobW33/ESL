/**********************************************************
 * This file is generated by 20-sim ANSI-C Code Generator
 *
 *  file:  common\xxfuncs.h
 *  model: Jiwy-1
 *  expmt: Jiwy-1
 *  date:  June 7, 2024
 *  time:  10:50:01 AM
 *  user:  Universiteit Twente
 *  from:  -
 *  build: 5.1.1.13146
 **********************************************************/

#ifndef XX_FUNCS_H
#define XX_FUNCS_H

/* Our own include files */
#include "xxtypes.h"

/* Wrapper functions around strings */
XXDouble XXString2Double(const char* argument);
const char* XXDouble2String(XXDouble argument);

/* 20-sim function prototypes */
XXDouble XXAbsolute (XXDouble argument);
XXDouble XXArcCosineHyperbolic (XXDouble argument);
XXDouble XXArcSineHyperbolic (XXDouble argument);
XXDouble XXArcTangentHyperbolic (XXDouble argument);
XXDouble XXExponent2 (XXDouble argument);
XXDouble XXExponent10 (XXDouble argument);
XXDouble XXIntegerDivide (XXDouble argument1, XXDouble argument2);
XXDouble XXIntegerModulo (XXDouble argument1, XXDouble argument2);
XXDouble XXLogarithm2 (XXDouble argument);
XXDouble XXLogarithm10 (XXDouble argument);
XXDouble XXPow2 (XXDouble argument);
XXDouble XXPower (XXDouble argument1, XXDouble argument2);
XXDouble XXRandom (XXDouble argument);
XXDouble XXRamp (XXDouble argument, XXDouble time);
XXDouble XXRound (XXDouble argument);
XXDouble XXSign (XXDouble argument);
XXDouble XXStep (XXDouble argument, XXDouble time);
XXDouble XXImpulse (XXDouble impulsestarttime, XXDouble impulseduration, XXDouble currenttime, XXDouble stepsize);
XXDouble XXXor (XXDouble argument1, XXDouble argument2);
/* XXDelay is model specific and therefore implemented in PositionControllerTilt.cpp */
/* XXInitialValue is model specific and therefore implemented in PositionControllerTilt.cpp */

XXInteger XXBitAnd(XXInteger argument1, XXInteger argument2);
XXInteger XXBitOr(XXInteger argument1, XXInteger argument2);
XXInteger XXBitXor(XXInteger argument1, XXInteger argument2);
XXInteger XXBitCmp(XXInteger argument, XXInteger nrBits);
XXInteger XXBitGet(XXInteger argument, XXInteger bitPos);
XXInteger XXBitSet(XXInteger argument1, XXInteger bitPos);
XXInteger XXBitInv(XXInteger argument);
XXInteger XXBitClear(XXInteger argument1, XXInteger bitPos);
XXInteger XXBitShift(XXInteger argument, XXInteger bitsToShift);
XXInteger XXBitShiftRight(XXInteger argument, XXInteger bitsToShift);
XXInteger XXSwapBytes(XXInteger argument);

/* 20-sim stub prototypes */
XXDouble XXData (XXString name, XXInteger column, XXInteger id);
XXDouble XXTable (XXString name, XXDouble argument, XXInteger id);
XXBoolean XXFrequencyEvent (XXDouble argument, XXInteger id);
XXBoolean XXFrequencyEvent1 (XXDouble argument1, XXDouble argument2, XXInteger id);
XXDouble XXTimeDelay (XXDouble argument, XXDouble time, XXInteger id);
XXBoolean XXWarning (XXString message, XXInteger id);
/* XXStopSimulation is implemented in PositionControllerTilt.cpp */
XXDouble XXRealTime(void);
#endif

