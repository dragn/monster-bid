/*
 * =====================================================================================
 *
 *       Filename:  cpu_emulator.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/06/2011 01:47:59 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "cpu_emulator.h"
#include <stdio.h>

using namespace mb;
using namespace std;

/*
 *--------------------------------------------------------------------------------------
 *       Class:  CpuEmulator
 *      Method:  CpuEmulator
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
CpuEmulator::CpuEmulator ()
{
}  /* -----  end of method CpuEmulator::CpuEmulator  (constructor)  ----- */


boost::shared_array<t_data>
CpuEmulator::single_run ( NetworkPtr net, boost::shared_array<t_data> input )
{
	boost::shared_array<t_data> output( new t_data[net->get_output_size()] );

	boost::shared_array<t_data> map( new t_data[net->get_size()] );
	boost::shared_array<t_data> map1( new t_data[net->get_size()] );

	uint size = net->get_size();
	uint input_size = net->get_input_size();
	uint output_size = net->get_output_size();

	for (uint i = 0; i < size; i++){
		map[i] = 0;
		map1[i] = 0;
	}

	for (uint i = 0; i < input_size; i++){
		map[i] = input[i];
	}

	bool stop = false;

	while (!stop){	

		stop = true;
		// process signal flows
		for (uint i = 0; i < size; i++){
			if (map[i] != 0){
				for (uint j = 0; j < size; j++){
					t_weight w = net->get_weight(i, j);
					if (w != 0){
						map1[j] += map[i] * w;
						stop = false;
					}
				}
				if (!stop) map[i] = 0;
			}
		}
		// apply sigmas
		for (uint i = 0; i < size; i++){
			if (map1[i] != 0){
				map[i] = 1.0 / ( 1.0 + exp(- ( map1[i] + net->get_bias(i) ) ));
			//	map[i] = map1[i];
				map1[i] = 0;
			}
		}	
	}

	for (uint i = 0; i < output_size; i++){
		output[i] = map[size - output_size + i];
	}

	return output;
}		/* -----  end of method CpuEmulator::single_run  ----- */

