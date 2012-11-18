/*
 * =====================================================================================
 *
 *       Filename:  cpu_emulator.h
 *
 *    Description:  Network emulator working on CPU only.
 *
 *        Version:  1.0
 *        Created:  12/06/2011 01:45:04 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef CPU_EMULATOR_H
#define CPU_EMULATOR_H

#include "common/common_include.h"
#include "emulator.h"
#include <boost/shared_array.hpp>
#include <math.h>

namespace mb {

	/*
	 * =====================================================================================
	 *        Class:  CpuEmulator
	 *  Description:  
	 * =====================================================================================
	 */
	class CpuEmulator: public Emulator
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			CpuEmulator ();                             /* constructor */

			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */
			boost::shared_array<t_data> single_run(NetworkPtr net, boost::shared_array<t_data> input);

		protected:
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  DATA MEMBERS  ======================================= */

	}; /* -----  end of class CpuEmulator  ----- */

}

#endif
