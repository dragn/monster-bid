/*
 * =====================================================================================
 *
 *       Filename:  cpu_tutor_algorithms.h
 *
 *    Description:  The impementation of TutorAlgorithms interface on CPU
 *
 *        Version:  1.0
 *        Created:  12/13/2011 03:46:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef CPU_TUTOR_ALGORITHMS_H
#define CPU_TUTOR_ALGORITHMS_H

#include "tutor_algorithms.h"
#include "emulation/emulator_frontend.h"

namespace mb{

	/*
	 * =====================================================================================
	 *        Class:  CPUTutorAlgorithms
	 *  Description:  
	 * =====================================================================================
	 */
	class CPUTutorAlgorithms : public TutorAlgorithms
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			CPUTutorAlgorithms ();                             /* constructor */

			/* ====================  OPERATORS     ======================================= */
			std::vector<NetworkPtr> reproduction(std::vector<NetworkPtr> pool, float ratio);
			void mutation(std::vector<NetworkPtr> pool, float ratio, float power);
			virtual std::vector<float> examination(std::vector<NetworkPtr> pool, std::vector<TestCasePtr> tests);
			std::vector<NetworkPtr> selection(std::vector<NetworkPtr> pool, std::vector<float> scores, float ratio);

			NetworkPtr make_child( NetworkPtr, NetworkPtr, float );
			void mutate( NetworkPtr, float );

			// Run provided testcase on provided network, return resulted score.
			float test( NetworkPtr, TestCasePtr );

			// Fitness function
			virtual float fitness( TestCasePtr, boost::shared_array<t_data> result );

		protected:
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  DATA MEMBERS  ======================================= */
			boost::shared_ptr<EmulatorFrontend> emulator;

	}; /* -----  end of class CPUTutorAlgorithms  ----- */

}

#endif
