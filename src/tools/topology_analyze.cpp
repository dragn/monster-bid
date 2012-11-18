/*
 * =====================================================================================
 *
 *       Filename:  topology_analyzer.cpp
 *
 *    Description:  Implementation of topology analyzing tools.
 *
 *        Version:  1.0
 *        Created:  12/05/2011 02:23:12 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "topology_analyze.h"

using namespace mb;

int TopologyAnalyze::analyze_and_get_depth ( Topology* tp )
{
	//TODO
	/* Simple wave algorithm ... */
	boost::shared_array<int> net( new int[tp->get_total_size()] );
	for (int i = 0; i < tp->get_total_size(); i++){
		net[i] = 0;
	}
	for (int i = 0; i < tp->get_input_size(); i++){
		net[i] = 1;
	}
	bool stop = false;
	int step = 1;
	while( !stop ){
		stop = true;
		for (int i = 0; i < tp->get_total_size(); i++){
			if (net[i] == step){
				for (int j = 0; j < tp->get_total_size(); j++){
					if (net[j] == 0 && tp->is_connected(i, j)){
						net[j] = step + 1;
						stop = false;
					}
				}
			}
		}
		step++;
	}
	int depth = 0;

	/* Checking for unreached output nodes */
	for (int i = tp->get_total_size() - tp->get_output_size(); i < tp->get_total_size(); i++){
		if (net[i] == 0){
			LOG(WARNING) << "Error in topology: output node " << i << " will not be reached by signal!";	
			depth = -1;
		}
	}
	if (depth == -1){
		return -1;
	}

	depth = net[tp->get_total_size() - tp->get_output_size()];
	for (int i = tp->get_total_size() - tp->get_output_size() + 1; i < tp->get_total_size(); i++){
		if (net[i] != depth){
			LOG(WARNING) << "Error in topology: output nodes will not be reached simultaneously";
			return -1;
		}
	}
	
	return depth;
}		/* -----  end of method TopologyAnalyze::get_depth  ----- */

