/*
 * =====================================================================================
 *
 *       Filename:  cpu_tutor_algorithms_alpha.h
 *
 *    Description:  CPUTutorAlgorithms for Alpha networks
 *
 *        Version:  1.0
 *        Created:  04/14/2012 01:32:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "common/common_include.h"
#include "cpu_tutor_algorithms.h"

namespace mb {

	/*
	 * =====================================================================================
	 *        Class:  CPUTutorAlgorithmsAlpha
	 *  Description:  l
	 * =====================================================================================
	 */
	class CPUTutorAlgorithmsAlpha : public CPUTutorAlgorithms
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			CPUTutorAlgorithmsAlpha ();                             /* constructor */

			/* ====================  ACCESSORS     ======================================= */

			/* ====================  MUTATORS      ======================================= */

			/* ====================  OPERATORS     ======================================= */
			virtual float fitness( TestCasePtr, boost::shared_array<t_data> result );

		protected:
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  DATA MEMBERS  ======================================= */

	}; /* -----  end of class CPUTutorAlgorithmsAlpha  ----- */

}
