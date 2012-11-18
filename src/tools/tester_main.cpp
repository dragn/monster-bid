/*
 * =====================================================================================
 *
 *       Filename:  tester_main.cpp
 *
 *    Description:  Tester program runs network on provided test cases file and
 *    				shows resulted fitness and success rate.
 *
 *        Version:  1.0
 *        Created:  01/29/2012 03:35:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <iomanip>

#include "common/common_include.h"
#include "education/testcase_loader.h"
#include "network/network.h"
#include "education/cpu_tutor_algorithms.h"

#include "tools/network_test.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
namespace algo = boost::algorithm;

using namespace mb;

void print_help(po::options_description cmd_opts){
	using namespace std;
	cout << "Monster Bid Tester is a tool to run a bunch of tests from file on a saved network" << endl;
	cout << "Usage: mbtester -I <input_file> -N <network_file>" << endl << endl;
	cout << cmd_opts;
}

void doRunnerMode(NetworkPtr net){
	boost::shared_ptr<EmulatorFrontend> emul( new EmulatorFrontend() );
	int input_size = net->get_input_size();
	int output_size = net->get_output_size();
	std::cout << "Successfully started in runner mode. Input size for loaded network: " <<
		input_size << ". \nEnter 'q' to quit. Waiting for input..." << std::endl;
	std::string line;
	boost::shared_array<t_data> input(new t_data[input_size]);
	boost::shared_array<t_data> output;
	int count = 0;
	std::vector<std::string> temp;
	while(true){
		std::cin >> line;
		if (line[0] == 'q') break;
		temp.clear();
		algo::split( temp, line, algo::is_any_of(" ") );
		try{
			foreach(std::string word, temp){
				t_data data = boost::lexical_cast<t_data>(word);
				input[count++] = data;
				if (count == input_size){
					boost::shared_array<t_data> output = emul->single_run(net, input);
					for(int i = 0; i < output_size; i++){
						std::cout << output[i] << " ";
					}
					std::cout << std::endl;
					count = 0;
				}
			}
		}catch (boost::bad_lexical_cast &ex){}
	}
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	google::InitGoogleLogging(argv[0]);

	po::options_description cmd_opts("MBTester command line options");
	cmd_opts.add_options()
		("input,I", po::value<std::string>(), "Tests file")
		("network,N", po::value<std::string>()->required(), "Network description file file (*.mbn)")
		("runner,R", "Runner mode - read input from stdin stream and write output to stdout.")
		("network-type,T", po::value<std::string>()->default_value("omega"), "Network type. Supported:\nomega (default) - simple network for exact output.\nalpha - stock market prediction. Output is a min-max range.")
		("help", "Show help");
	po::variables_map vm;

	po::command_line_parser cmd_parser(argc,argv);
	cmd_parser.style(po::command_line_style::unix_style);

	try{
		cmd_parser.options(cmd_opts);
		po::store(cmd_parser.run(), vm);

		if (vm.count("help") != 0){
			print_help(cmd_opts);
			return EXIT_SUCCESS;
		}

		po::notify(vm);

		if (vm.count("runner") == 0 and vm.count("input") == 0){
			LOG(FATAL) << "Input file option required in default mode. (without -R, -I <filename> must be supplied)";
		}
	} catch( po::required_option err ){
		print_help(cmd_opts);
		return EXIT_SUCCESS;
	} catch( po::validation_error err ){
		LOG(FATAL) << "Option validation error: " << err.get_option_name();
	}

	bool runnerMode = (vm.count("runner") != 0);

	// -- loading test
	//
	std::vector<TestCasePtr> tests;
	if (!runnerMode){
		std::cout << "Loading tests..." << std::endl;
		tests = TestCaseLoader::load_from_file( vm["input"].as<std::string>() );	
		if (tests.size() == 0){
			LOG(FATAL) << "No tests loaded!";
		}
	}

	// -- loading network
	//
	fs::path net_path( vm["network"].as<std::string>() );
	if ( !fs::exists(net_path) ){
		LOG(FATAL) << "Network file not found: " << net_path;
	}
	fs::ifstream net_file( net_path, std::ios_base::in );
	NetworkPtr net( new Network( &net_file ) );
	net_file.close();

	if (runnerMode){
		doRunnerMode(net);
	}else{
		//std::cout << "Loaded network: " << std::endl;
		//net->print( &LOG(INFO) );

		if (tests[0]->get_input_size() != net->get_input_size() ||
				tests[0]->get_output_size() != net->get_output_size()){
			LOG(FATAL) << "Network's and tests' input/output sizes must be equal!";
		}

		std::string type = vm["network-type"].as<std::string>();

		EmulatorFrontend emulator;

		if (type == "omega"){
			// -- Omega network testing
			//
			CPUTutorAlgorithms ta;

			int test_number = 0;
			float summ = 0;
			foreach( TestCasePtr test, tests ){
				float score = ta.test( net, test );
				boost::shared_array<float> output = emulator.single_run( net, test->get_input() );
				for (uint i = 0; i < net->get_output_size(); i++){
					std::cout << "Network: " << output[i] << std::endl;
					std::cout << "Test: " << test->get_output()[i] << std::endl;
				}
				summ += score;
				std::cout << "Scores on test #" << test_number++ << ": " << score << std::endl;
			}

			std::cout << "Square mean: " << sqrt(summ) / tests.size() << std::endl;
			std::cout << "Square mean descrepancy: " << 100 * sqrt(summ) / tests.size() << "%"<< std::endl;
		} else if (type == "alpha"){
			// -- Alpha network testing
			//
			LOG_IF( FATAL, net->get_output_size() != 2 ) << "Alpha network should have 2 outputs!";

			boost::shared_array<t_data> output;

			float reliability = 0.0;
			float advantage = 0.0;

			float x1; float x2;
			float y1; float y2;
			
			foreach( TestCasePtr test, tests ){
				output = emulator.single_run(net, test->get_input());
				x1 = output[0];
				x2 = output[1];
				y1 = test->get_output()[0];
				y2 = test->get_output()[1];

				std::cout << "Network: " << x1 << ", " << x2 << std::endl;
				std::cout << "Test: " << y1 << ", " << y2 << std::endl << std::endl;

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
				if (x1 < x2){
					advantage += (y2 - y1) / (x2 - x1) * 100.0;
				}
			}

			reliability /= tests.size();
			advantage /= tests.size();

			std::cout << "Got reliability: " << reliability << "%" << std::endl;
			std::cout << "And advantage: " << advantage << "%" << std::endl;
		}
	}


	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
