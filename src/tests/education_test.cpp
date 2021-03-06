/*
 * =====================================================================================
 *
 *       Filename:  education_test.cpp
 *
 *    Description:  Education package testing routines
 *
 *        Version:  1.0
 *        Created:  12/13/2011 01:33:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <boost/lexical_cast.hpp>
#include <string.h>
#include <vector>
#include "education/cpu_tutor_algorithms_alpha.h"

#ifdef CUDA_ENABLED
#include "education/cuda_tutor_algorithms.h"
#endif

#include "education/tutor.h"

#define ASSERT(condition, message) LOG_IF(FATAL, !(condition)) << "Test #" << test << " failed: " << message;
#define START_TEST(msg) LOG(INFO) << "------------"; LOG(INFO) << "Starting Test #" << ++test << ". " << msg;

using namespace mb;

void print_test_case( TestCasePtr test ){
	std::string str = "";
	boost::shared_array<t_data> tc_input = test->get_input();
	for (uint i = 0; i < test->get_input_size(); i++){
		str += boost::lexical_cast<std::string>( tc_input[i] ) + ", ";		
	}
	LOG(INFO) << "  Input: [" << str << "]";
	
	str = "";
	boost::shared_array<t_data> tc_output = test->get_output();
	for (uint i = 0; i < test->get_output_size(); i++){
		str += boost::lexical_cast<std::string>( tc_output[i] ) + ", ";		
	}
	LOG(INFO) << "  Output: [" << str << "]";
}

#ifdef CUDA_ENABLED
void compare_algos(Tutor tr, boost::shared_ptr<TutorAlgorithms> cpu_algos, 
		boost::shared_ptr<TutorAlgorithms> cuda_algos, std::vector<TestCasePtr> tests, int &test){
	LOG(INFO) << "    Creating random pool...";
	std::vector<NetworkPtr> pool;
	NetworkPtr net_ptr;
	for (uint i = 0; i < 40; i++){
		net_ptr.reset( new Network(tr.get_topology().get(), 1, 1) );
		pool.push_back( net_ptr );
	}
	LOG(INFO) << "    Starting CPU examination";
	clock_t clk = clock();
	std::vector<float> scores = cpu_algos->examination(pool, tests);
	float cpu_score_summ = 0;
	foreach( float f, scores ){
		cpu_score_summ += f;
//		LOG(INFO) << f;
	}
	LOG(INFO) << "    CPU Finished. Score summ: " << cpu_score_summ << ". Elapsed: " << (float)(clock() - clk)/CLOCKS_PER_SEC << "s.";
	LOG(INFO) << "    Starting CUDA examination";
	clk = clock();
	scores = cuda_algos->examination(pool, tests);
	float cuda_score_summ = 0;
	foreach( float f, scores ){
		cuda_score_summ += f;
//		LOG(INFO) << f;
	}
	LOG(INFO) << "    CUDA Finished. Score summ: " << cuda_score_summ << ". Elapsed: " << (float)(clock() - clk)/CLOCKS_PER_SEC << "s.";
	ASSERT(cpu_score_summ - cuda_score_summ < 1E-8, "Got different scores for CUDA and CPU!! ");

	boost::static_pointer_cast<CUDATutorAlgorithms>(cuda_algos)->free();

}
#endif

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main ( int argc, char *argv[] )
{
	google::InitGoogleLogging( argv[0] );
	google::InstallFailureSignalHandler();

	int test = 0;

	START_TEST( "Loading test cases." );
	std::vector<TestCasePtr> tests = TestCaseLoader::load_from_file( "data/mul_test.tsc" );
	ASSERT(tests.size() > 0, "No tests loaded! Check that file 'data/mul_test.tsc' exists!");

	START_TEST("Initializing Tutor.");
	Tutor tr( "conf/tutor.mul_test.conf" );

	START_TEST("Creating sample network by configured topology.");
	NetworkPtr net( new Network(tr.get_topology().get(), 1, 1) );

	START_TEST("CPU child making test.");
	boost::shared_ptr<CPUTutorAlgorithms> algos( new CPUTutorAlgorithms() );

	srand( time(NULL) );	

	NetworkPtr parent1( new Network( tr.get_topology().get(), 1, 1 ) );
	NetworkPtr parent2( new Network( tr.get_topology().get(), 1, 1 ) );
	NetworkPtr child = algos->make_child( parent1, parent2, 0.5 );

	for (uint i = 0; i < parent1->get_input_size(); i++){
		for (uint j = 0; j < parent1->get_input_size(); j++){
			t_data child_w = child->get_weight(i, j);
			ASSERT(child_w == parent1->get_weight(i, j) || child_w == parent2->get_weight(i, j), "Child creation error! i = " << i << ", j = " << j);
		}
		ASSERT(child->get_bias(i) == parent1->get_bias(i) || child->get_bias(i) == parent2->get_bias(i), "Child creating error! i = " << i);
	}

	START_TEST("Mutation test.");
	NetworkPtr net1( new Network(tr.get_topology().get(), 1, 1) );
	algos->mutate( net1, 0 );

/* 	START_TEST("Alpha networks algorithms test.");
	boost::shared_ptr<CPUTutorAlgorithms> alpha_algos( new CPUTutorAlgorithmsAlpha() );
	TestCasePtr testcase( new TestCase() );
	boost::shared_array<t_data> output(new t_data[2]);
	output[0] = 0.5;
	output[1] = 0.7;
	testcase->set_output(2, output.get());
	boost::shared_array<t_data> result(new t_data[2]);

	// When ranges do not intersect 
	result[0] = 0.3;
	result[1] = 0.4;
	float fitness = alpha_algos->fitness(testcase, result);
	ASSERT(fitness - 0.2 < 1e-4, "Fitness must be equal to 0.2! Got: " << fitness);

	// Invalid output
	result[0] = 0.7;
	result[1] = 0.4;
	fitness = alpha_algos->fitness(testcase, result);
	ASSERT(fitness == 1000, "Fitness must be equal to 1000! Got: " << fitness);

	// When ranges are equal, fitness must be equal to 0
	result[0] = 0.49999;
	result[1] = 0.70001;
	fitness = alpha_algos->fitness(testcase, result);
	ASSERT(fitness < 1e-4, "Fitness must be equal to 0! Got: " << fitness);

	// When result range contains test range, the fitness is (result - test) / result * 1000
	result[0] = 0.4;
	result[1] = 0.9;
	fitness = alpha_algos->fitness(testcase, result);
	ASSERT(fitness - 0.6 < 1e-6, "Fitness must be equal to 0.6! Got: " << fitness);

	// | |result| - |test| | + | |intersect| - |result| |
	// When ranges intersect, the general formula if (result - intersection) / result * 1000
	result[0] = 0.6;
	result[1] = 0.8;
	fitness = alpha_algos->fitness(testcase, result);
	ASSERT(fitness - 1.1 < 1e-6, "Fitness must be equal to 1.1! Got: " << fitness);

	// When ranges intersect, the general formula if (result - intersection) / result * 1000
	result[0] = 0.2;
	result[1] = 0.6;
	fitness = alpha_algos->fitness(testcase, result);
	ASSERT(fitness - 1.5 < 1e-6, "Fitness must be equal to 1.5! Got: " << fitness);

	// When result contained in target, the fitness is (test - result) / test * 1000
	result[0] = 0.55;
	result[1] = 0.65;
	fitness = alpha_algos->fitness(testcase, result);
	ASSERT(fitness - 2.1 < 1e-6, "Fitness must be equal to 2.1! Got: " << fitness);
 */
/*  	LOG(INFO) << test++ << ". Starting education.";
	tr.set_tests( tests );
	tr.educate();

	LOG(INFO) << test++ << ". Testing resulted network: ";
	NetworkPtr best = tr.get_best();
	best->print( &LOG(INFO) );

	EmulatorFrontend ef;

	for( uint t = 0; t < tests.size(); t++ ){
		LOG(INFO) << "--Test case #" << t;
		print_test_case( tests[t] );
		boost::shared_array<t_data> result = ef.single_run( best, tests[t]->get_input() );
		std::string result_str = "";
		for (uint i = 0; i < net->get_output_size(); i++){
			result_str += boost::lexical_cast<std::string>( result[i] ) + ", ";		
		}
		LOG(INFO) << "  Network output: [" << result_str << "]";
	} */

#ifdef CUDA_ENABLED
	START_TEST("Testing GPU-CPU code consistency");
	boost::shared_ptr<TutorAlgorithms> cuda_algos( new CUDATutorAlgorithms() );
	boost::shared_ptr<TutorAlgorithms> cpu_algos( new CPUTutorAlgorithms() );

	compare_algos(tr, cpu_algos, cuda_algos, tests, test);

	START_TEST("Testing GPU-CPU consistency on alpha networks");
	tests = TestCaseLoader::load_from_file( "data/alpha_test.tsc" );
	ASSERT(tests.size() > 0, "No tests loaded! Check that file 'data/alpha_test.tsc' exists!");
	Tutor tr_alpha( "conf/tutor.alpha_test.conf" );

	cuda_algos.reset( new CPUTutorAlgorithmsAlpha() );
	cpu_algos.reset( new CUDATutorAlgorithms("alpha") );

	compare_algos(tr_alpha, cpu_algos, cuda_algos, tests, test);

#endif

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */


