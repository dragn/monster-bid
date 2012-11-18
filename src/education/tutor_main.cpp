/*
 * =====================================================================================
 *
 *       Filename:  tutor_main.cpp
 *
 *    Description:  MB Tutor application main function
 *
 *        Version:  1.0
 *        Created:  01/09/2012 09:30:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include <stdlib.h>
#include <string>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "common/common_include.h"
#include "education/testcase_loader.h"
#include "education/tutor.h"

using namespace mb;

namespace po = boost::program_options;
namespace fs = boost::filesystem;

void print_help(po::options_description options){
	std::cout << "Monster Bid: financial neural network education software\n";
	std::cout << "Usage: mbtutor -I INPUT_FILE [-C CONFIG_FILE]\n\n";
	std::cout << options << std::endl;
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
	google::InitGoogleLogging( argv[0] );
	google::InstallFailureSignalHandler();

	po::options_description cmd_opts( "mbtutor command line options" );
	cmd_opts.add_options()
		("help", "Show help")
		("conf,C", po::value<std::string>(), "Tutor configuration file path, default: conf/tutor.conf")
		("input,I", po::value<std::string>()->required(), "Input file with education data")
		("network-name,N", po::value<std::string>(), "Resulted network filename (extension .mbn will be added)")
		("force,f", "Override network file if it already exists.");

	po::variables_map vm;

	// -- Parsing command line
	//
	try{
		po::command_line_parser cmd_parser(argc,argv);
		cmd_parser.style(po::command_line_style::unix_style);

		cmd_parser.options(cmd_opts);
		po::store(cmd_parser.run(), vm);

		po::notify(vm);
	} catch( po::required_option err ){
		print_help(cmd_opts);
		return EXIT_SUCCESS;
	} catch( po::validation_error err ){
		LOG(FATAL) << "Option validation error: " << err.get_option_name();
	}

	if (vm.count("help") != 0){
		print_help(cmd_opts);
		return EXIT_SUCCESS;
	}

	// -- Test file loading
	//
	std::cout << "Loading tests...";
	std::vector<TestCasePtr> tests = TestCaseLoader::load_from_file( vm["input"].as<std::string>() );
	std::cout << "done. " << tests.size() << " tests loaded" << std::endl;

	// -- Execution run
	//
	std::cout << "Executing education..." << std::endl;
	std::string config = "conf/tutor.conf";
	if (vm.count("conf") != 0) {
		config = vm["conf"].as<std::string>();
	}	
	Tutor tr(config);
	tr.set_tests( tests );
	tr.educate();
	
	// -- Saving resulted network
	//
	std::string filename = "unnamed.mbn";
	if (vm.count("network-name") != 0){
		filename = vm["network-name"].as<std::string>() + ".mbn";
	}
	fs::path path( filename );
	if ( fs::exists(path) && vm.count("force") == 0){
		std::string answer = "";
		std::cout << "File " << filename << " already exists. Overwrite? [y/N]: ";
		std::cin >> answer;
		if (answer != "y" && answer != "Y"){
			std::cout << "Network was not saved." << std::endl;
			return EXIT_SUCCESS;
		}
	}
	fs::ofstream os( filename, std::ios_base::out ); //TODO catch exception
	NetworkPtr best( tr.get_best() );
	if (best.get() != 0){
		best->serialize( &os );
		std::cout << "Network saved in '" << filename << "'." << std::endl;
	}

	std::cout << "Finished." << std::endl;

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
