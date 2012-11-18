/*
 * =====================================================================================
 *
 *       Filename:  testcase.cpp
 *
 *    Description:  TestCase class implementation
 *
 *        Version:  1.0
 *        Created:  12/13/2011 01:23:20 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "testcase.h"

using namespace mb;

/*
 *--------------------------------------------------------------------------------------
 *       Class:  TestCase
 *      Method:  TestCase
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
TestCase::TestCase ()
{
}  /* -----  end of method TestCase::TestCase  (constructor)  ----- */


void TestCase::set_input ( uint input_size, t_data *data )
{
	this->input_size = input_size;

	if (input.get() != 0){
		LOG(WARNING) << "Ignoring repeating set_input on testcase";
		return;
	}

	input.reset( new t_data[input_size] );
	for (uint i = 0; i < input_size; i++){
		input[i] = data[i];
	}
	return;
}		/* -----  end of method TestCase::set_input  ----- */


void TestCase::set_output ( uint output_size, t_data *data )
{
	this->output_size = output_size;

	if (output.get() != 0){
		LOG(WARNING) << "Ignoring repeating set_output on testcase";
		return;
	}

	output.reset( new t_data[output_size] );
	for (uint i = 0; i < output_size; i++){
		output[i] = data[i];
	}
	return ;
}		/* -----  end of method TestCase::set_output  ----- */


boost::shared_array<t_data>
TestCase::get_input ()
{
	return input;
}		/* -----  end of method TestCase::get_input  ----- */


boost::shared_array<t_data>
TestCase::get_output ()
{
	return output;
}		/* -----  end of method TestCase::get_output  ----- */


uint TestCase::get_input_size ()
{
	return input_size;
}		/* -----  end of method TestCase::get_input  ----- */


uint TestCase::get_output_size ()
{
	return output_size;
}		/* -----  end of method TestCase::get_output  ----- */

