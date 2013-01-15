/*
 * =====================================================================================
 *
 *       Filename:  cpu_tutor_algorithms.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/13/2011 03:55:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "cpu_tutor_algorithms.h"
#include <algorithm>

using namespace mb;


/*
 *--------------------------------------------------------------------------------------
 *       Class:  CPUTutorAlgorithms
 *      Method:  CPUTutorAlgorithms
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
CPUTutorAlgorithms::CPUTutorAlgorithms ()
{
	emulator.reset( new EmulatorFrontend() );
}  /* -----  end of method CPUTutorAlgorithms::CPUTutorAlgorithms  (constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  CPUTutorAlgorithms
 *      Method:  CPUTutorAlgorithms :: reproduction
 * Description:  Returns N * ratio networks designated from networks in pool. N is a 
 * 				 number of networks in the pool.
 *--------------------------------------------------------------------------------------
 */
std::vector<NetworkPtr> CPUTutorAlgorithms::reproduction ( std::vector<NetworkPtr> pool, float ratio )
{
	// New feature: differential reproduction.
	// This step actually does some mutation, so mutation step is not neaded.
	std::vector<NetworkPtr> _pool;
	uint size = (uint) pool.size() * ratio;
	std::random_shuffle( pool.begin(), pool.end() );
	int x1;
	int x2;
	for ( uint i = 0; i < size; i++ ){
		// for each N of first networks do
		//   randomly choose two other networks,
		//   calculate their differential and
		//   apply childing algorithm to the first network and differential
		NetworkPtr parent1 = pool[i];

		do { x1 = rand() % pool.size(); } while (x1 == i);
		NetworkPtr net1 = pool[x1]; 

		do { x2 = rand() % pool.size(); } while (x2 == i || x2 == x1);
		NetworkPtr net2 = pool[x2];

		assert( x1 != x2 && x1 != i && x2 != i );

		// Differentiate
		// foreach (i,j): diff_w(i,j) = net1_w(i,j) - net2_w(i,j)
		NetworkPtr diff( new Network(parent1->get_size(), parent1->get_input_size(), parent1->get_output_size(), parent1->get_weight_map(), parent1->get_bias_map()) );
		for (uint x = 0; x < diff->get_size(); x++){
			for (uint y = 0; y < diff->get_size(); y++){
				diff->set_weight(x, y, 0.5 * ( net1->get_weight(x, y) - net2->get_weight(x, y) ) );
			}
		}
		for (uint x = 0; x < diff->get_size(); x++){
			diff->set_bias(x, 0.5 * ( net1->get_bias(x) - net2->get_bias(x) ) );
		}

		_pool.push_back( make_child( parent1, diff, 0.5 ) );
	}
	return _pool;
}		/* -----  end of method CPUTutorAlgorithms::reproduction  ----- */

NetworkPtr 
CPUTutorAlgorithms::make_child( NetworkPtr parent1, NetworkPtr parent2, float parm ){
	//TODO;
	// Simplest algorithm:
	// Step through all connections, throw a dice do determine which connection
	// will go to child, step through all biases.
	if ( parent1->get_size() != parent2->get_size() ||
			parent1->get_input_size() != parent2->get_input_size() ||
			parent1->get_output_size() != parent2->get_output_size() ){
		LOG(INFO) << parent1->get_size() << " != " << parent2->get_size();
		LOG(INFO) << parent1->get_input_size() << " != " << parent2->get_input_size();
		LOG(INFO) << parent1->get_output_size() << " != " << parent2->get_output_size();
		LOG(FATAL) << "Trying to make a child from a networks of different sizes!";
	}
	uint size = parent1->get_size();

	// Generate weights map
	boost::shared_array<t_weight> child_weights( new t_weight[ size * size ] );

	for (uint i = 0; i < size; i++){
		for (uint j = 0; j < size; j++){			
			if (rand() / RAND_MAX < parm && parent1->get_weight(i,j) != 0){
				child_weights[ j*size + i ] = parent1->get_weight(i, j); 			
			}else{
				child_weights[ j*size + i ] = parent2->get_weight(i, j); 			
			}
		}
	}

	// Generate biases map
	boost::shared_array<t_weight> child_biases( new t_weight[ size ] );

	for (uint i = 0; i < size; i++){
		if (rand() / RAND_MAX < parm && parent1->get_bias(i) != 0){
			child_biases[i] = parent1->get_bias(i);
		}else{
			child_biases[i] = parent2->get_bias(i);
		}

	}

	NetworkPtr child( new Network( size, parent1->get_input_size(), parent1->get_output_size(),
				child_weights.get(), child_biases.get() ) );

	return child;
}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  CPUTutorAlgorithms
 *      Method:  CPUTutorAlgorithms :: mutation
 * Description:  Performs a random mutation on N * ratio randomly chosen networks from pool.
 *--------------------------------------------------------------------------------------
 */
void CPUTutorAlgorithms::mutation ( std::vector<NetworkPtr> pool, float ratio, float power )
{
	// TODO;
	for (uint i = 0; i < ratio * pool.size(); i++){
		int index = rand() % pool.size();
		mutate( pool[index], power );
	}
}		/* -----  end of method CPUTutorAlgorithms::mutation  ----- */

	void
CPUTutorAlgorithms::mutate ( NetworkPtr network, float power )
{
	// Simplest algorithm:
	// randomly choose a connection and apply a random multiplier from range 0~2
	// the same with biases.

	uint count = 1;

	if (power < (network->get_size() * network->get_size()) / 2){
		count += power;
	}

	float coef_range = 2;

	float coef = 1;
	uint size = network->get_size();

	for (uint ix = 0; ix < count; ix++){
		uint i = 0; 
		uint j = 0;
		do{
			i = rand() % size;
			j = rand() % size;
		} while (network->get_weight(i, j) == 0);

		// Generate coefficient
		coef = (2 * ( (float) rand() / RAND_MAX ) - 1) * coef_range;

		// Apply new weight
		network->set_weight( i, j, coef * network->get_weight(i, j) );
	}

	coef = (2 * ( (float) rand() / RAND_MAX ) - 1) * coef_range;

	// Biases mutation
	coef = (float) rand() / RAND_MAX;
	uint index = rand() % size;
	network->set_bias( index, coef * network->get_bias(index) );
}		/* -----  end of method CPUTutorAlgorithms::mutate  ----- */


/*
 *--------------------------------------------------------------------------------------
 *       Class:  CPUTutorAlgorithms
 *      Method:  CPUTutorAlgorithms :: examination
 * Description:  Executes every test on every network and calculates their scores.
 * 				 Returns network scores on defined tests.
 *--------------------------------------------------------------------------------------
 */
	std::vector<float>
CPUTutorAlgorithms::examination ( std::vector<NetworkPtr> pool, std::vector<TestCasePtr> tests )
{
	// TODO
	// CPU version: test every network one by one on every test
	std::vector<float> scores;
	for (uint i = 0; i < pool.size(); i++){
		float score = 0;
		for (uint t = 0; t < tests.size(); t++){
			score += test( pool[i], tests[t] );
		}
		scores.push_back( sqrt(score) / tests.size() );
	}
	return scores;
}		/* -----  end of method CPUTutorAlgorithms::examination  ----- */

	float
CPUTutorAlgorithms::test ( NetworkPtr network, TestCasePtr test )
{
	assert( network->get_input_size() == test->get_input_size() );
	boost::shared_array<t_data> output = emulator->single_run( network, test->get_input() );
	return fitness( test, output );
}		/* -----  end of method CPUTutorAlgorithms::test  ----- */


	float
CPUTutorAlgorithms::fitness ( TestCasePtr test, boost::shared_array<t_data> output )
{
	// Summarize square mean deviations for every output
	float result = 0;
	boost::shared_array<t_data> target = test->get_output();
	for (uint i = 0; i < test->get_output_size(); i++){
		t_data delta = output[i] / target[i] - 1;
		result += delta * delta;
	}
	return result;
}		/* -----  end of method CPUTutorAlgorithms::fitness  ----- */


/*
 *--------------------------------------------------------------------------------------
 *       Class:  CPUTutorAlgorithms
 *      Method:  CPUTutorAlgorithms :: selection
 * Description:  Returns N * ratio networks selected by defined scores
 *--------------------------------------------------------------------------------------
 */
	std::vector<NetworkPtr>
CPUTutorAlgorithms::selection ( std::vector<NetworkPtr> pool, std::vector<float> scores, float ratio )
{
	// TODO
	// Tournament algorithm: pick every two networks and let the strongest survive
	std::vector<NetworkPtr> _pool;
	for (uint i = 0; i < pool.size() / 2; i++){
		NetworkPtr net1 = pool.at(i * 2);
		NetworkPtr net2 = pool.at(i * 2 + 1);
		if (scores.at(i * 2) < scores.at(i * 2 + 1)){
			_pool.push_back(net1);
		} else {
			_pool.push_back(net2);
		}
	}

	//	uint size = pool.size() * ratio;
	assert( _pool.size() == pool.size() / 2 );
	return _pool;
}		/* -----  end of method CPUTutorAlgorithms::selection  ----- */


