/*
 * =====================================================================================
 *
 *       Filename:  tutor_algorithms.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/13/2011 03:42:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef TUTOR_ALGORITHMS_H_
#define TUTOR_ALGORITHMS_H_

#include "common/common_include.h"
#include "network/network.h"
#include <vector>
#include "testcase.h"

namespace mb{

	/*
	 * =====================================================================================
	 *        Class:  TutorAlgorithms
	 *  Description:  
	 * =====================================================================================
	 */
	class TutorAlgorithms
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			TutorAlgorithms () {};                             /* constructor */

			/* ====================  OPERATORS     ======================================= */

			/*
			 *--------------------------------------------------------------------------------------
			 *       Class:  TutorAlgorithms
			 *      Method:  TutorAlgorithms :: reproduction
			 * Description:  Returns N * ratio networks designated from networks in pool. N is a 
			 * 				 number of networks in the pool.
			 *--------------------------------------------------------------------------------------
			 */
			virtual std::vector<NetworkPtr> reproduction(std::vector<NetworkPtr> pool, float ratio) = 0;

			/*
			 *--------------------------------------------------------------------------------------
			 *       Class:  TutorAlgorithms
			 *      Method:  TutorAlgorithms :: mutation
			 * Description:  Performs a random mutation on N * ratio randomly chosen networks from pool.
			 *--------------------------------------------------------------------------------------
			 */
			virtual void mutation(std::vector<NetworkPtr> pool, float ratio, float power) = 0;

			/*
			 *--------------------------------------------------------------------------------------
			 *       Class:  TutorAlgorithms
			 *      Method:  TutorAlgorithms :: examination
			 * Description:  Executes every test on every network and calculates their scores.
			 * 				 Returns network scores on defined tests.
			 *--------------------------------------------------------------------------------------
			 */
			virtual std::vector<float> examination(std::vector<NetworkPtr> pool, std::vector<TestCasePtr> tests) = 0;

			/*
			 *--------------------------------------------------------------------------------------
			 *       Class:  TutorAlgorithms
			 *      Method:  TutorAlgorithms :: selection
			 * Description:  Returns N * ratio networks selected by defined scores
			 *--------------------------------------------------------------------------------------
			 */
			virtual std::vector<NetworkPtr> selection(std::vector<NetworkPtr> pool, std::vector<float> scores, float ratio) = 0;

		protected:
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  DATA MEMBERS  ======================================= */

	}; /* -----  end of class TutorAlgorithms  ----- */

}

#endif
