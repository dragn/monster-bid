/*
 * =====================================================================================
 *
 *       Filename:  network_test.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/23/2012 10:50:32 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  daniel.sabelnikov (), dsabelnikov@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "common/common_include.h"
#include "network/network.h"
#include "education/testcase.h"
#include "emulation/emulator_frontend.h"

namespace mb {

	/*
	 * =====================================================================================
	 *        Class:  NetworkTest
	 *  Description:  
	 * =====================================================================================
	 */
	class NetworkTest
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			NetworkTest ();                             /* constructor */

			static float getReliability(NetworkPtr network, std::vector<TestCasePtr> tests);
			static float getAdvantage(NetworkPtr network, std::vector<TestCasePtr> tests);
		protected:
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  DATA MEMBERS  ======================================= */

	}; /* -----  end of class NetworkTest  ----- */

}
