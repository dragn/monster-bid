/*
 * =====================================================================================
 *
 *       Filename:  emulator_frontend.h
 *
 *    Description:  Emulator frontend
 *
 *        Version:  1.0
 *        Created:  12/06/2011 01:21:58 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef EMULATOR_FRONTEND_H
#define EMULATOR_FRONTEND_H

#include "common/common_include.h"
#include "network/network.h"
#include <vector>
#include "cpu_emulator.h"

namespace mb {

	class Emulator;

	/*
	 * =====================================================================================
	 *        Class:  EmulatorFrontend
	 *  Description:  
	 * =====================================================================================
	 */
	class EmulatorFrontend
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			EmulatorFrontend ();                             /* constructor */

			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */
			boost::shared_array<t_data> single_run(NetworkPtr net, boost::shared_array<t_data> input);

		protected:
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  DATA MEMBERS  ======================================= */
			boost::shared_ptr<Emulator> choose_emulator();

			boost::shared_ptr<Emulator> cpu_emulator;			

	}; /* -----  end of class EmulatorFrontend  ----- */
}

#endif
