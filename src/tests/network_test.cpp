/*
 * =====================================================================================
 *
 *       Filename:  test_network.cpp
 *
 *    Description:  Network implementation testing program
 *
 *        Version:  1.0
 *        Created:  12/03/2011 12:52:41 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniel (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include "common/common_include.h"
#include "network/network.h"
#include "emulation/emulator_frontend.h"

#include "tools/topology_factory.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace mb;

namespace fs = boost::filesystem;

#define ASSERT(condition, message) LOG_IF(FATAL, !(condition)) << "Test #" << test << " failed: " << message;
#define START_TEST(msg) LOG(INFO) << "------------"; LOG(INFO) << "Starting Test #" << ++test << ". " << msg;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	google::InitGoogleLogging( argv[0] );
	google::InstallFailureSignalHandler();

	int test = 0;

	LOG(INFO) << "Executing tests of Network and Topology implementation...";

	START_TEST("Creating an instance of Topology: Topology(7,2,2)");
	Topology tp(7, 2, 2); 
	ASSERT(tp.get_total_size() == 7, "Invalid total size of topology!");
	ASSERT(tp.get_input_size() == 2, "Invalid input size of topology!");
	ASSERT(tp.get_output_size() == 2, "Invalid output size of topology!");

	START_TEST("Creating a default instance of Topology: Topology()");
	Topology tp1(7,2,2); 
	LOG(INFO) << "   Created: total_size: " << tp1.get_total_size() << ", input_size = " 
				<< tp1.get_input_size() << ", output_size = " << tp1.get_output_size();

	START_TEST("Adding connections: [0,1]-[2,3,4]-[5,6]");
	tp.add_connection(0, 2);
	tp.add_connection(0, 3);
	tp.add_connection(0, 4);

	tp.add_connection(1, 2);
	tp.add_connection(1, 3);
	tp.add_connection(1, 4);

	tp.add_connection(2, 5);
	tp.add_connection(3, 5);
	tp.add_connection(4, 5);

	tp.add_connection(2, 6);
	tp.add_connection(3, 6);
	tp.add_connection(4, 6);

	ASSERT( !tp.is_connected(0, 1),"Error 0-1");
	ASSERT( tp.is_connected(0, 2), "Error 0-2");
	ASSERT( tp.is_connected(0, 3), "Error 0-3");
	ASSERT( tp.is_connected(1, 2), "Error 1-2");
	ASSERT( tp.is_connected(1, 4), "Error 1-4");
	ASSERT( !tp.is_connected(0, 5), "Error 0-5");
	ASSERT( !tp.is_connected(5, 6), "Error 5-6");

	START_TEST("Analyze and get depth. ");
	ASSERT( TopologyAnalyze::analyze_and_get_depth(&tp) == 3, "Invalid depth got from analyzer!");

	START_TEST("Creating a random network with max_weight = 1.0, and bias = 0.5 ...");
	NetworkPtr net( new Network(&tp, 1.0, 0.5) );
	for (uint i = 0; i < tp.get_total_size(); i++){
		for (uint j = 0; j < tp.get_total_size(); j++){
			t_weight w = net->get_weight(i, j);
			ASSERT(w >= -1.0 && w <= 1.0, "Weight was found more than 1.0!");
		}
	}
	//net->print( &LOG(INFO) );

	START_TEST("Emulator testing!");
	
	EmulatorFrontend ef;

	boost::shared_array<t_data> input( new t_data[3] );
	input[0] = 1;
	input[1] = 1;
	input[2] = 1;

	net->set_weight(0, 2, 0.5);
	net->set_weight(0, 3, 0.5);
	net->set_weight(0, 4, 0.5);

	net->set_weight(1, 2, 0.5);
	net->set_weight(1, 3, 0.5);
	net->set_weight(1, 4, 0.5);

	net->set_weight(2, 5, 0.33333333);
	net->set_weight(3, 5, 0.33333333);
	net->set_weight(4, 5, 0.33333333);

	net->set_weight(2, 6, 0.33333333);
	net->set_weight(3, 6, 0.33333333);
	net->set_weight(4, 6, 0.33333333);

	net->set_bias(0, 0);
	net->set_bias(1, 0);
	net->set_bias(2, 0);
	net->set_bias(3, 0);
	net->set_bias(4, 0);
	net->set_bias(5, 0);
	net->set_bias(6, 0);

	t_data expected = 1 / (1 + exp(-1));
	expected = 1 / (1 + exp(-expected));
	
	boost::shared_array<t_data> output = ef.single_run(net, input);

	ASSERT(output[0] == expected, "Didn't get expected output! output: " << output[0] << ", expected: " << expected);
	ASSERT(output[1] == expected, "Didn't get expected output! output: " << output[1] << ", expected: " << expected);
	
	input[0] = 0;
	input[1] = 0;
	input[2] = 0;
	
	output = ef.single_run(net, input);

	ASSERT(output[0] == 0, "Didn't get expected output! output: " << output[0] << ", expected: " << 0);
	ASSERT(output[1] == 0, "Didn't get expected output! output: " << output[1] << ", expected: " << 0);

	// -- with biases
	//
	input[0] = 1;
	input[1] = 1;
	input[2] = 1;

	net->set_bias(2, 0.2);
	net->set_bias(3, 0.2);
	net->set_bias(4, 0.2);

	net->set_bias(5, 0.1);
	net->set_bias(6, 0.6);

	expected = 1 / (1 + exp( - 1.2));
	t_data expected2 = 1 / (1 + exp( - expected - 0.6));
	expected = 1 / (1 + exp( - expected - 0.1));

	output = ef.single_run(net, input);

	ASSERT(output[0] == expected, "Didn't get expected output! output: " << output[0] << ", expected: " << expected);
	ASSERT(output[1] == expected2, "Didn't get expected output! output: " << output[1] << ", expected: " << expected2);

	START_TEST("Creating a topology with TopologyFactory.");

	std::vector<uint> layers;
	layers.push_back( 3 );
	layers.push_back( 2 );

	boost::shared_ptr<Topology> tp2( 
		TopologyFactory::create_connected_layers_topology(1, 1, layers) );

	NetworkPtr net2( new Network( tp2.get(), 1.0, 1.0 ) );

	ASSERT(net2->get_weight(0,1) != 0, "Wrong topology");
	ASSERT(net2->get_weight(0,2) != 0, "Wrong topology");
	ASSERT(net2->get_weight(0,3) != 0, "Wrong topology");
	ASSERT(net2->get_weight(0,4) == 0, "Wrong topology");
	ASSERT(net2->get_weight(0,5) == 0, "Wrong topology");
	ASSERT(net2->get_weight(5,6) != 0, "Wrong topology");
	ASSERT(net2->get_weight(4,6) != 0, "Wrong topology");
	ASSERT(net2->get_weight(3,6) == 0, "Wrong topology");

	START_TEST("Testing serialization algorithms.");
	fs::path net_file("./___test.mbn");
	fs::ofstream out_file( net_file, std::ios_base::out );
	net2->serialize( &out_file );
	out_file.close();

	fs::ifstream in_file( net_file, std::ios_base::in );
	NetworkPtr net3( new Network( &in_file ) );
	in_file.close();

	for (uint i = 0; i < tp.get_total_size(); i++){
		for (uint j = 0; j < tp.get_total_size(); j++){
			ASSERT( net2->get_weight(i, j) == net3->get_weight(i, j), "Serialization error! i = " << i << ", j = " << j << ", net1: " << net2->get_weight(i, j) << ", net2: " << net3->get_weight(i, j));
		}
		ASSERT( net2->get_bias(i) == net3->get_bias(i), "Serialization error! i = " << i);
	}
	
    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
