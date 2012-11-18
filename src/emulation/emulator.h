/*
 * =====================================================================================
 *
 *       Filename:  emulator.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/06/2011 01:52:23 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "common/common_include.h"
#include "network/network.h"
#include <boost/shared_array.hpp>

namespace mb {	
	/*
	 * =====================================================================================
	 *        Class:  Emulator
	 *  Description:  Base class for all emulators
	 * =====================================================================================
	 */
	class Emulator
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			Emulator ();                             /* constructor */

			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */
			virtual boost::shared_array<t_data> single_run(NetworkPtr net, boost::shared_array<t_data> input) = 0;

		protected:
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  DATA MEMBERS  ======================================= */

	}; /* -----  end of class Emulator  ----- */

}
