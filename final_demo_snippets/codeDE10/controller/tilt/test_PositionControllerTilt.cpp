/**********************************************************
 * This file is generated by 20-sim C++ Code Generator
 *
 *  file:  test_PositionControllerTilt.cpp
 *  subm:  PositionControllerTilt
 *  model: Jiwy-1
 *  expmt: Jiwy-1
 *  date:  June 7, 2024
 *  time:  10:50:01 AM
 *  user:  Universiteit Twente
 *  from:  -
 *  build: 5.1.1.13146
 **********************************************************/

/* This file is a demo application of how the submodel function can
 * be used. It uses the global time variables that are used by both
 * the submodel and the integration method.
 *
 * PLEASE NOTE: THIS IS AN EXAMPLE WHERE ALL INPUTS ARE ZERO !
 * USE YOUR OWN INPUTS INSTEAD!! ALSO THE SUBMODEL MIGHT SIMPLY
 * NOT WORK CORRECTLY WITH INPUTS THAT ARE ZERO.
 */

#include <stdio.h>

/* 20-sim submodel class include file */
#include "PositionControllerTilt.h"

/* the main function */
int main()
{
	XXDouble u [3 + 1];
	XXDouble y [1 + 1];

	/* initialize the inputs and outputs with correct initial values */
	u[0] = 0.0;		/* corr */
	u[1] = 0.0;		/* in */
	u[2] = 0.0;		/* position */

	y[0] = 0.0;		/* out */


	PositionControllerTilt my20simSubmodel;

	/* initialize the submodel itself and calculate the outputs for t=0.0 */
	my20simSubmodel.Initialize(u, y, 0.0);
	printf("Time: %f\n", my20simSubmodel.GetTime() );

	/* simple loop, the time is incremented by the integration method */
	while (my20simSubmodel.state != PositionControllerTilt::finished)
	{
		/* call the submodel to calculate the output */
		my20simSubmodel.Calculate (u, y);
		printf("Time: %f\n", my20simSubmodel.GetTime() );
	}

	/* perform the final calculations */
	my20simSubmodel.Terminate (u, y);

	/* and we are done */
	return 0;
}

