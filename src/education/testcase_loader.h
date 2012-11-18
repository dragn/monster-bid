/*
 * =====================================================================================
 *
 *       Filename:  testcase_loader.h
 *
 *    Description:  TestCaseLoader is a class containing routines for loading
 *    				testcases from files and another sources
 *
 *        Version:  1.0
 *        Created:  12/13/2011 10:31:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef TESTCASE_LOADER_H_
#define TESTCASE_LOADER_H_

#include "testcase.h"
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace mb{
	/*
	 * =====================================================================================
	 *        Class:  TestCaseLoader
	 *  Description:  
	 * =====================================================================================
	 */
	class TestCaseLoader
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			TestCaseLoader ();                             /* constructor */

			/* ====================  ACCESSORS     ======================================= */
			static std::vector<TestCasePtr> load_from_file( std::string filepath );
		private:
			static std::vector<std::string> get_lines( boost::filesystem::ifstream* ifs );
	}; /* -----  end of class TestCaseLoader  ----- */

}

#endif
