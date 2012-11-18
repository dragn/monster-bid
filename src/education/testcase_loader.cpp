/*
 * =====================================================================================
 *
 *       Filename:  testcase_loader.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/15/2011 02:22:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "testcase_loader.h"

#include "common/common_include.h"
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

namespace fs = boost::filesystem;
namespace algo = boost::algorithm;

using namespace mb;

/*
 *--------------------------------------------------------------------------------------
 *       Class:  TestCaseLoader
 *      Method:  TestCaseLoader :: load_from_file
 * Description:  Load test cases from specified file.
 *
 * Data file format:
 *  'FILENAME:' keyword must be specified first, all lines before it will be ignored.
 *  Then 'INPUT_SIZE:' and 'OUTPUT_SIZE:' must be specified.
 *  Then starts data section with input and output data specified by 'IN:' and 'OUT:' keywords.
 *--------------------------------------------------------------------------------------
 */
std::vector<TestCasePtr>
TestCaseLoader::load_from_file ( std::string filepath )
{	
	std::vector<TestCasePtr> tests;

	fs::path file ( filepath );
	if ( !fs::exists( file ) ){
		LOG(WARNING) << "Data file not found! No tests loaded!";
		return tests;
	}

	/* Defining regexp's */
	boost::regex filename_exp( "^ *FILENAME: *(.*) *$" );
	boost::regex input_size_exp( "^ *INPUT_SIZE: *([0-9]+) *$" );
	boost::regex output_size_exp( "^ *OUTPUT_SIZE: *([0-9]+) *$" );
	boost::regex in_exp( "^ *IN: *(.*) *$" );
	boost::regex out_exp( "^ *OUT: *(.*) *$" );
	boost::regex uint_exp( "^[0-9]*$" );
	boost::regex float_array_exp( "^ *([-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)? *)+ *$" );

	/* ----------------- */


	fs::ifstream ifs( file, std::ios_base::in );
	std::vector<std::string> lines = get_lines( &ifs );
	
	/* Look for a FILENAME: keyword first */
	std::string filename = "";
	std::string line = "";
	uint index = 0;
	while ( index < lines.size() ){
		line = lines[index++];
		if ( boost::regex_match(line, filename_exp) ){			
			filename = boost::regex_replace( line, filename_exp,
				"\\1", boost::match_default | boost::format_sed );
			break;
		}
	}
	if (filename == ""){
		LOG(WARNING) << "'FILENAME:' keyword not found in test cases specification. No tests loaded.";
		return tests;
	}

	/* Get input_size and output_size from file, keywords INPUT_SIZE and OUTPUT_SIZE */
	uint input_size = 0;
	uint output_size = 0;
	while ( index < lines.size() ){
		line = lines[index++];
		if ( boost::regex_match( line, input_size_exp ) ){
			std::string input_size_str = boost::regex_replace(
				line, input_size_exp, "\\1", boost::match_default | boost::format_sed );
			input_size = boost::lexical_cast<uint>( input_size_str );
			if (output_size != 0){
				break;
			}
		}
		if ( boost::regex_match( line, output_size_exp ) ){
			std::string output_size_str = boost::regex_replace(
				line, output_size_exp, "\\1", boost::match_default | boost::format_sed );
			output_size = boost::lexical_cast<uint>( output_size_str );
			if (input_size != 0){
				break;
			}
		}
	}

	if ( input_size == 0 || output_size == 0 ){
		LOG(WARNING) << filename << ": Input or output sizes not found in test cases file. " <<
			"Check that 'INPUT_SIZE:' and 'OUTPUT_SIZE:' keywords present BEFORE data section." <<
			"Values may be less than or equal to zero or in incorrect format.";
		return tests;
	}

	/* 
	 * Parse test cases:
	 *   for all lines do
	 *   	if line starts with IN: 
	 *   		append data to input buffer
	 *   	if line starts with OUT:
	 *   		append data to output buffer
	 *   		check if input data size valid - no less then input_size
	 *   			and output data size valid - no less then output_size
	 *   		if not, throw a warning and clear buffers (in and out)
	 *   		else create testcase and set data
	 *   	at the end check both buffers and create last testcase 
	 */
	boost::shared_array<t_data> input( new t_data[input_size] );
	boost::shared_array<t_data> output( new t_data[output_size] );
	bool valid_input = false;
	bool valid_output = false;
	while ( index < lines.size() ){
		line = lines[index++];

		/*  IN:  */
		if ( boost::regex_match( line, in_exp ) ){
			valid_input = false;
			std::string input_str = boost::regex_replace( line, in_exp,
				"\\1", boost::match_default | boost::format_sed );
			if ( boost::regex_match( input_str, float_array_exp ) ){
				std::vector<std::string> temp;
				algo::split( temp, input_str, algo::is_any_of(" ") );
				if ( temp.size() < input_size ){
					LOG(WARNING) << filename << ": Invalid input size at line " << index;
				} else {
					for (uint i = 0; i < input_size; i++){
						input[i] = boost::lexical_cast<t_data>( temp[i] );
					}
					valid_input = true;
				}
			}else{
				LOG(WARNING) << filename << ": Invalid input format at line " << index;
			}
		}

		/*  OUT:  */
		if ( valid_input && boost::regex_match( line, out_exp ) ){
			valid_output = false;
			std::string output_str = boost::regex_replace( line, out_exp,
				"\\1", boost::match_default | boost::format_sed );
			if ( boost::regex_match( output_str, float_array_exp ) ){
				std::vector<std::string> temp;
				algo::split( temp, output_str, algo::is_any_of(" ") );
				if ( temp.size() < output_size ){
					LOG(WARNING) << filename << ": Invalid output size at line " << index;
				} else {
					for (uint i = 0; i < output_size; i++){
						output[i] = boost::lexical_cast<t_data>( temp[i] );
					}
					valid_output = true;
				}
			}else{
				LOG(WARNING) << filename << ": Invalid output format at line " << index;
			}

			if (valid_output){
				/* Create new test case */
				TestCasePtr test( new TestCase() );
				test->set_input( input_size, input.get() );
				test->set_output( output_size, output.get() );
				tests.push_back( test );
			}
		}

	}

	return tests;
}		/* -----  end of method TestCaseLoader::load_from_file  ----- */

std::vector<std::string>
TestCaseLoader::get_lines( fs::ifstream* ifs ){
	std::stringstream ss;
	ss << ifs->rdbuf();
	std::string str = ss.str();

	std::vector<std::string> lines;
	algo::split( lines, str, algo::is_any_of("\n") );
	return lines;
}
