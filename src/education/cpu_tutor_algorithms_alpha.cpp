/*
 * =====================================================================================
 *
 *       Filename:  cpu_tutor_algorithms_alpha.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/14/2012 01:34:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "cpu_tutor_algorithms_alpha.h"
#include <cmath>

using namespace mb;


/*
 *--------------------------------------------------------------------------------------
 *       Class:  CPUTutorAlgorithmsAlpha
 *      Method:  CPUTutorAlgorithmsAlpha
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
CPUTutorAlgorithmsAlpha::CPUTutorAlgorithmsAlpha ()
{
}  /* -----  end of method CPUTutorAlgorithmsAlpha::CPUTutorAlgorithmsAlpha  (constructor)  ----- */


float
CPUTutorAlgorithmsAlpha::fitness( TestCasePtr test, boost::shared_array<t_data> result )
{
	float score = 0;
	assert( test->get_output_size() == 2 );
	boost::shared_array<t_data> target = test->get_output();
	// | |result| - |test| | + | |intersect| - |result| |
	if ( result[1] < result[0]){
		// -- first value greater than second
		//
		score = 1000;
	} else {
		t_data z1 = 0;
		t_data z2 = 0;
		t_data x1 = target[0];
		t_data x2 = target[1];
		t_data y1 = result[0];
		t_data y2 = result[1];
		if ( x1 <= y1 && y1 <= x2 ){
			z1 = y1;
			score += 1; // this case is undesired;
		} else if ( y1 <= x1 && x1 <= y2 ) {
			z1 = x1;
		}
		if ( x1 <= y2 && y2 <= x2 ){
			z2 = y2;
			score += 1; // this case is undesired;
		} else if ( y1 <= x2 && x2 <= y2 ) {
			z2 = x2;
		}
		score += ( fabs( (x2 - x1) - (y2 - y1) ) + fabs( (z2 - z1) - (y2 - y1) ) ) / (y2 - y1);
	}

	return score;
}		/* -----  end of method CPUTutorAlgorithms::fitness  ----- */

