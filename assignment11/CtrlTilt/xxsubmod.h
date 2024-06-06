/**********************************************************
 * This file is generated by 20-sim ANSI-C Code Generator
 *
 *  file:  xxsubmod.h
 *  subm:  PositionControllerTilt
 *  model: Jiwy-1
 *  expmt: Jiwy-1
 *  date:  June 5, 2024
 *  time:  1:57:35 PM
 *  user:  Universiteit Twente
 *  from:  -
 *  build: 5.1.1.13146
 **********************************************************/

/* This file describes the model functions
   that are supplied for computation.

   The model itself is the xxmodel.c file
*/

#ifndef XX_SUBMOD_H
#define XX_SUBMOD_H

/* Our own include files */
#include "xxmodel.h"

/* Submodel I/O variables */
extern XXInteger xx_number_of_inputs;
extern XXInteger xx_number_of_outputs;

/* The names of the variables as used in the arrays above */
extern XXString xx_input_names[];
extern XXString xx_output_names[];


/* The submodel functions */
void XXInitializeSubmodel (XXDouble *u, XXDouble *y, XXDouble t);
void XXCalculateSubmodel (XXDouble *u, XXDouble *y, XXDouble t);
void XXTerminateSubmodel (XXDouble *u, XXDouble *y, XXDouble t);
void XXCopyInputsToVariables (XXDouble *u);
void XXCopyVariablesToOutputs (XXDouble *y);

#endif

