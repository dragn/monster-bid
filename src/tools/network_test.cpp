/*
 * =====================================================================================
 *
 *       Filename:  network_test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/23/2012 10:52:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  daniel.sabelnikov (), dsabelnikov@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "network_test.h"

using namespace mb;

float 
NetworkTest::getReliability(NetworkPtr net, std::vector<TestCasePtr> tests){
	EmulatorFrontend emulator;
	boost::shared_array<t_data> output;

	float reliability = 0.0;

	float x1; float x2;
	float y1; float y2;

	foreach( TestCasePtr test, tests ){
		output = emulator.single_run(net, test->get_input());
		x1 = output[0];
		x2 = output[1];
		y1 = test->get_output()[0];
		y2 = test->get_output()[1];

		if (x1 > y2 || x2 < y1 || x2 < x1){
			// -- No intersection at all!
			//
			reliability += 0;
		}else if (x1 < y1 && x2 > y2){
			// -- (x1,x2) containts (y1,y2)
			//
			reliability += 100.0;
		}else if (x1 > y1){
			reliability	+= 100.0 * (y2 - x1) / (y2 - y1);				
		}else {
			reliability += 100.0 * (x2 - y1) / (y2 - y1);
		}
	}

	reliability /= tests.size();
	return reliability;
}


float 
NetworkTest::getAdvantage(NetworkPtr net, std::vector<TestCasePtr> tests){
	EmulatorFrontend emulator;
	boost::shared_array<t_data> output;

	float advantage = 0.0;

	float x1; float x2;
	float y1; float y2;

	foreach( TestCasePtr test, tests ){
		output = emulator.single_run(net, test->get_input());
		x1 = output[0];
		x2 = output[1];
		y1 = test->get_output()[0];
		y2 = test->get_output()[1];

		if (x1 < x2){
			advantage += (y2 - y1) / (x2 - x1) * 100.0;
		}
	}

	advantage /= tests.size();
	return advantage;
}
