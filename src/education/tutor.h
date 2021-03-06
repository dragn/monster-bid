/*
 * =====================================================================================
 *
 *       Filename:  tutor.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/13/2011 08:29:24 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "common/common_include.h"
#include "testcase_loader.h"
#include "tutor_algorithms.h"
#include <boost/program_options.hpp>
#include "tools/network_test.h"

namespace po = boost::program_options;

namespace mb{

	/*
	 * =====================================================================================
	 *        Class:  Tutor
	 *  Description:  The class that performs neural network education 
	 * =====================================================================================
	 */
	class Tutor
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */
			Tutor( std::string config_file );

			/* ====================  ACCESSORS     ======================================= */

			/*
			 *--------------------------------------------------------------------------------------
			 *       Class:  Tutor
			 *      Method:  Tutor :: get_best
			 * Description:  Returns the best result of education
			 *--------------------------------------------------------------------------------------
			 */
			NetworkPtr get_best();

			const boost::shared_ptr<po::options_description> get_options();
			const boost::shared_ptr<Topology> get_topology();

			/* ====================  MUTATORS      ======================================= */
			void set_tests(std::vector<TestCasePtr> tests);

			/* ====================  OPERATORS     ======================================= */

			/*
			 *--------------------------------------------------------------------------------------
			 *       Class:  Tutor
			 *      Method:  Tutor :: educate
			 * Description:  Start the education process
			 *--------------------------------------------------------------------------------------
			 */
			void educate();

		protected:
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			Tutor ();                             /* constructor */

			/* ====================  DATA MEMBERS  ======================================= */
			std::vector<NetworkPtr> pool;
			std::vector<NetworkPtr> _pool;

			std::vector<NetworkPtr> sorted_pool;

			std::vector<TestCasePtr> tests;

			boost::shared_ptr<po::options_description> options;
			po::variables_map options_vm;
			std::string config_file;

			void init_options();
			void init();
			boost::shared_ptr<TutorAlgorithms> init_algorithms();

			boost::shared_ptr<Topology> topology;
			boost::shared_ptr<TutorAlgorithms> algorithms;

			bool is_CUDA();
	}; /* -----  end of class Tutor  ----- */

}
