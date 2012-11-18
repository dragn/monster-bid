/*
 * =====================================================================================
 *
 *       Filename:  testcase.h
 *
 *    Description:  TestCase is a class representing a single testcase
 *    				for neural network education.
 *
 *        Version:  1.0
 *        Created:  12/12/2011 09:15:04 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef TESTCASE_H_
#define TESTCASE_H_

#include <boost/shared_array.hpp>
#include <common/common_include.h>

namespace mb{

	/*
	 * =====================================================================================
	 *        Class:  TestCase
	 *  Description:  Test case for neural network education.
	 *  			  Contains an input date and appropriate output.
	 * =====================================================================================
	 */
	class TestCase
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			TestCase ();                             /* constructor */

			/* ====================  ACCESSORS     ======================================= */
			boost::shared_array<t_data> get_input();
			boost::shared_array<t_data> get_output();

			uint get_input_size();
			uint get_output_size();

			/* ====================  MUTATORS      ======================================= */
			void set_input(uint input_size, t_data *input_data);
			void set_output(uint output_size, t_data *output_data);

		private:
			/* ====================  DATA MEMBERS  ======================================= */
			uint input_size;
			uint output_size;
			boost::shared_array<t_data> input;
			boost::shared_array<t_data> output;


	}; /* -----  end of class TestCase  ----- */

	/*
	 *	Type definition of a pointer to TestCase instance.
	 */
	typedef boost::shared_ptr<TestCase> TestCasePtr;
}

#endif
