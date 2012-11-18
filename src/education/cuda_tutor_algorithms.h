/*
 * =====================================================================================
 *
 *       Filename:  cuda_tutor_algorithms.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/14/2012 10:02:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  daniel.sabelnikov (), dsabelnikov@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef CUDA_TUTOR_ALGORITHMS_H
#define CUDA_TUTOR_ALGORITHMS_H

#include "cpu_tutor_algorithms.h"

#include "cuda.h"
#include "cuda_runtime.h"

namespace mb{
	/*
	 * =====================================================================================
	 *        Class:  CUDATutorAlgorithms
	 *  Description:  
	 * =====================================================================================
	 */
	class CUDATutorAlgorithms : public CPUTutorAlgorithms
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			CUDATutorAlgorithms ();                             /* constructor      */
			CUDATutorAlgorithms (std::string network_type) : network_type(network_type), allocated(false) {};     /* constructor      */

			/* ====================  OPERATORS     ======================================= */
			virtual std::vector<float> examination(std::vector<NetworkPtr> pool, std::vector<TestCasePtr> tests);

			// Release any allocated memory
			void free();
		protected:
			/* ====================  DATA MEMBERS  ======================================= */
			void allocate(int network_size, int input_size, int output_size, int pool_size, int tests_size);
		private:
			/* ====================  DATA MEMBERS  ======================================= */
			bool allocated;
			std::string network_type;
			
			/* Device pointer: d_test_input 
			 * Contains inputs for each test. 
			 * Size: input_size * tests.size() */
			float *d_test_input; size_t d_test_input_pitch;

			/* Device pointer: d_test_output
			 * Contains outputs for each test.
			 * Size: output_size * tests.size() */
			float *d_test_output; size_t d_test_output_pitch;

			/* Device pointer: d_networks
			 * Contains weights of each network in pool
			 * Size: network_size^2 * pool_size */
			cudaPitchedPtr d_networks;

			/* Device pointer: d_biases
			 * Contains biases for networks
			 * Size: network_size * pool_size */
			float* d_biases; size_t d_biases_pitch;

			/* Device pointer: d_calc
			 * Used by kernel for calculations on each network
			 * Size: 2 * network_size * pool_size */
			float *d_calc; size_t d_calc_pitch;

			/* Device pointer: d_scores
			 * Contains output of scores calculations in kernels
			 * Size: pool_size */
			float *d_scores;
	}; /* -----  end of class CUDATutorAlgorithms  ----- */

}

#endif
