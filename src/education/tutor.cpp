/*
 * =====================================================================================
 *
 *       Filename:  tutor.cpp
 *
 *    Description:  Tutor class implementation
 *
 *        Version:  1.0
 *        Created:  12/13/2011 08:35:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "tutor.h"

#include "common/common_include.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include "tools/topology_factory.h"
#include <boost/tokenizer.hpp>
#include "cpu_tutor_algorithms.h"
#include "cpu_tutor_algorithms_alpha.h"

#ifdef CUDA_ENABLED
#include "cuda_tutor_algorithms.h"
#endif

#include <utility>

using namespace mb;
using namespace std;

namespace fs = boost::filesystem;
namespace algo = boost::algorithm;

typedef boost::tokenizer< boost::escaped_list_separator<char> > t_tokenizer;

/* Constants */
const static char* DEVICE_OPTION = "computation-device";
const static char* POOL_SIZE_OPTION = "pool-size";
const static char* TOPOLOGY_TYPE_OPTION = "topology-type";
const static char* TOPOLOGY_INPUT_SIZE_OPTION = "topology-input-size";
const static char* TOPOLOGY_OUTPUT_SIZE_OPTION = "topology-output-size";
const static char* CONNECTED_LAYERS_TYPE = "connected-layers";
const static char* CONNECTED_LAYERS_SIZES = "connected-layers-sizes";
const static char* STEPS_OPTION = "steps";
const static char* MAX_WEIGHT_OPTION = "max-weight";
const static char* MAX_BIAS_OPTION = "max-bias";
const static char* NETWORK_TYPE_OPTION = "network-type";

const static boost::regex UINT_REGEX( "[+]?[0-9]+" );

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Tutor
 *      Method:  Tutor
 * Description:  constructor
 * 				 During class initialization configuration will be parsed from a 
 * 				 configuration file.
 *--------------------------------------------------------------------------------------
 */
Tutor::Tutor () : config_file( "conf/tutor.conf" )
{
	init();
}  /* -----  end of method Tutor::Tutor  (constructor)  ----- */
Tutor::Tutor ( string config_file ) : config_file( config_file )
{
	init();
}  /* -----  end of method Tutor::Tutor  (constructor)  ----- */


void Tutor::init ()
{
	LOG(INFO) << "Tutor initializaton begins...";
	// Parse parameters from config file
	fs::path config_path( config_file );

	if ( fs::exists( config_path ) ){
		try{
			fs::ifstream config_ifs( config_path, ios_base::in ); 
			po::store( po::parse_config_file( config_ifs, *(get_options()), true), options_vm );
			config_ifs.close();
			po::notify( options_vm );
		} catch (po::required_option err) {
			LOG(FATAL) << "Required option not found: " << err.get_option_name();
		} catch (po::validation_error err) {
			LOG(FATAL) << "Option value invalid: " << err.get_option_name();
		}
	} else {
		LOG(FATAL) << "Tutor config file " << config_file << " not found!";
	}

	// Initializing topology

	/* Topology type */
	string topology_type = options_vm[ TOPOLOGY_TYPE_OPTION ].as<string>();

	/* Input size */
	uint input_size = options_vm[ TOPOLOGY_INPUT_SIZE_OPTION ].as<uint>();

	/* Output size */
	uint output_size = options_vm[ TOPOLOGY_OUTPUT_SIZE_OPTION ].as<uint>();

	/* Type-specific initialization */

	if (topology_type == CONNECTED_LAYERS_TYPE){

		/* Connected layers sizes */
		if (options_vm.count(CONNECTED_LAYERS_SIZES) < 1){
			LOG(FATAL) << "Configuration error: \"" << CONNECTED_LAYERS_SIZES << "\" option is required for connected layers topology";
		}
		string layers_sizes = options_vm[ CONNECTED_LAYERS_SIZES ].as< string >();
		t_tokenizer tok( layers_sizes );

		vector<uint> layers;
		foreach( string s, make_pair( tok.begin(), tok.end() ) ){
			try{
				layers.push_back( boost::lexical_cast<uint>( s ) );
			}catch(boost::bad_lexical_cast err){
				LOG(WARNING) << "Configuration error: \"" << CONNECTED_LAYERS_SIZES <<
					"\", invalid format value \"" << s << "\" skipped";
			}
		};

		topology = TopologyFactory::create_connected_layers_topology(input_size, output_size, layers);
	}

	LOG(INFO) << "Tutor initialization finished successfully.";
}		/* -----  end of method Tutor::init  ----- */


	NetworkPtr
Tutor::get_best ()
{
	// TODO
	NetworkPtr empty;
	if (pool.size() == 0){
		return empty;
	}
	NetworkPtr best = pool[0];
	float r_max = 0;
	foreach(NetworkPtr net, pool){
		float r = NetworkTest::getReliability(net, tests);
		if (r > r_max){
			r_max = r;
			best = net;
		}
	}
	return best;
}		/* -----  end of method Tutor::get_best  ----- */


void Tutor::set_tests ( vector<TestCasePtr> tests )
{
	this->tests = tests;
}		/* -----  end of method Tutor::set_tests  ----- */

	const boost::shared_ptr<po::options_description>
Tutor::get_options ()
{
	if (options.get() == 0){
		init_options();
	}
	return options;
}		/* -----  end of method Tutor::get_options  ----- */


	const boost::shared_ptr<Topology>
Tutor::get_topology ()
{
	return topology;
}		/* -----  end of method Tutor::get_topology  ----- */


void Tutor::init_options ()
{
	po::options_description* opt = new po::options_description("Tutor options");
	opt->add_options()
		( DEVICE_OPTION, po::value<string>(), "CPU or CUDA")
		( TOPOLOGY_TYPE_OPTION, po::value<string>()->required(), "Type of topology, currently supported: connected-layers")
		( TOPOLOGY_INPUT_SIZE_OPTION, po::value<uint>()->required(), "Input layer size")
		( TOPOLOGY_OUTPUT_SIZE_OPTION, po::value<uint>()->required(), "Output layer size")
		( CONNECTED_LAYERS_SIZES, po::value< string >(), "'connected-layers' topology settings: sizes of layers, delimeted by commas or whitespaces.")
		( POOL_SIZE_OPTION, po::value< uint >()->required(), "Network pool size, the number of networks in one generation")
		( STEPS_OPTION, po::value<uint>()->required(), "The number of education steps to perform")
		( MAX_WEIGHT_OPTION, po::value<t_weight>(), "The maximum connection weight for generated networks")
		( MAX_BIAS_OPTION, po::value<t_weight>(), "The maximum bias for generated networks")
		( NETWORK_TYPE_OPTION, po::value<string>()->default_value("omega"), "The type of the neuro-network");
	options.reset( opt );
}		/* -----  end of method Tutor::init_options  ----- */


	boost::shared_ptr<TutorAlgorithms>
Tutor::init_algorithms ()
{
	boost::shared_ptr<TutorAlgorithms> ptr;
	string device = options_vm[ DEVICE_OPTION ].as< string >();

#ifndef CUDA_ENABLED
	if(device == "CUDA"){
		LOG(WARNING) << "Tutor has been compiled without CUDA support, 'CUDA' device not allowed. Falling back to CPU.";
		device = "CPU";
	}
#endif
	if (device != "CPU" && device != "CUDA"){
		LOG(WARNING) << "Specified computation device " << device << " is not supported. Using default - CPU.";
		device = "CPU";
	}

	string network_type = options_vm[ NETWORK_TYPE_OPTION ].as<string>();

	if (device == "CPU"){
		if (network_type == "alpha"){
			LOG(INFO) << "Network type: Alpha";
			ptr.reset( new CPUTutorAlgorithmsAlpha() ); 
		} else {
			LOG(INFO) << "Network type: Omega";
			ptr.reset( new CPUTutorAlgorithms() ); 
		}
	} else if (device == "CUDA"){
#ifdef CUDA_ENABLED
		if (network_type == "alpha"){
			LOG(INFO) << "Network type: Alpha";
			ptr.reset( new CUDATutorAlgorithms("alpha") );
		} else {
			LOG(INFO) << "Network type: Omega";
			ptr.reset( new CUDATutorAlgorithms() );
		}
#endif
	}
	return ptr;
}		/* -----  end of method Tutor::init_algorithms  ----- */


bool
Tutor::is_CUDA ()
{
	string device = options_vm[ DEVICE_OPTION ].as< string >();
	return device == "CUDA";
}		/* -----  end of method Tutor::is_CUDA  ----- */



/*
 *--------------------------------------------------------------------------------------
 *       Class:  Tutor
 *      Method:  Tutor :: educate
 * Description:  Perform neural network education. This method calls the TutorAlgorithms
 * 				 routines to perform following steps in education process:
 * 				 1. reproduction() - fill certain amount of next generation pool with
 * 				    children originated from current pool.
 * 				 2. mutation() - perform a rendom mutation on current generation pool.
 * 				 3. examination() - test all network with loaded testcases,
 * 				    result of the examination will be returned in an array of scores
 * 				    that will be used to select the best network on next step.
 * 				 4. selection() - do a natural selection, this is a partly rendom process
 * 				    that selects best networks according to their scores at examination.
 * 				 5. check if stop condition emerged.
 *--------------------------------------------------------------------------------------
 */
void Tutor::educate ()
{
	if (tests.size() == 0){
		LOG(WARNING) << "No test cases loaded. Ignoring call to education()...";
		return;
	}

	LOG(INFO) << "Education process start.";

	boost::shared_ptr<TutorAlgorithms> algos = init_algorithms();	
	uint pool_size = options_vm[ POOL_SIZE_OPTION ].as< uint >();

	float reproduction_ratio = 0.5;
	float mutation_ratio = 0.5;

	// -- Load Max weight and max bias from options
	//
	t_weight max_weight = 1;
	t_weight max_bias = 1;

	if ( options_vm.count( MAX_WEIGHT_OPTION ) != 0 ){
		max_weight = options_vm[ MAX_WEIGHT_OPTION ].as<t_weight>();
	}
	if ( options_vm.count( MAX_BIAS_OPTION ) != 0 ){
		max_bias = options_vm[ MAX_BIAS_OPTION ].as<t_weight>();
	}

	uint steps = options_vm[ STEPS_OPTION ].as<uint>();

	// -- Clear network pools
	//
	pool.clear();
	_pool.clear();

	// -- Fill first pool with random networks
	//
	srand( time(NULL) );
	NetworkPtr *ptr;
	for (uint i = 0; i < pool_size; i++){
		ptr = new NetworkPtr( new Network(topology.get(), max_weight, max_bias) ); 
		pool.push_back( *ptr );
	}

	// -- Education cycle start
	//
	uint step = 1;
	while (true){

		LOG(INFO) << " -- Starting step #" << step;
		clock_t step_start = clock();

		// -- 1. Reproduction.
		//
		foreach( NetworkPtr ptr, algos->reproduction( pool, reproduction_ratio ) ){
			_pool.push_back(ptr);
		}
		// FIXME experimentail feature: throwing in two new networks
		if (_pool.size() > 2){
			_pool.pop_back();
			_pool.pop_back();
			_pool.push_back( *(new NetworkPtr(new Network(topology.get(), max_weight, max_bias))) );
			_pool.push_back( *(new NetworkPtr(new Network(topology.get(), max_weight, max_bias))) );
		}
		assert( pool.size() == pool_size );

		// -- 2. Mutation.
		//
//		algos->mutation( _pool, mutation_ratio, step/100 );
		algos->mutation( pool, mutation_ratio, 0 );
		assert( pool.size() == pool_size );

		// -- 3. Examination.
		//
		vector<float> scores = algos->examination( pool, tests );
		assert( pool.size() == pool_size );

		// -- Print scores
		//
		vector<float> sorted_scores;
		sorted_pool.clear();
		for(uint i = 0; i < scores.size(); i++){
			float score = scores[i];
			vector<float>::iterator iter = lower_bound(sorted_scores.begin(), sorted_scores.end(), score);
			int index = iter - sorted_scores.begin();
			sorted_scores.insert(iter, score);
			sorted_pool.insert(sorted_pool.begin() + index, pool[i]);
		}
		LOG(INFO) << "  Sorted scores:";
		for (uint i = 0; i < 5 && i < sorted_scores.size(); i++){
			LOG(INFO) << "    " << sorted_scores[i];
		}
 
		// -- 4. Selection.
		//
		foreach( NetworkPtr ptr, algos->selection( pool, scores, 1 - reproduction_ratio ) ){
			_pool.push_back( ptr );
		}

		assert( pool.size() == pool_size );
		assert( _pool.size() == pool_size );

		// -- 5. Check stop condition
		//
		if (step >= steps){
			break;
		}

		// -- Increment step counter
		step++;

		// -- Swap pools
		pool.clear();
		foreach( NetworkPtr ptr, _pool ){
			pool.push_back( ptr );
		}
		_pool.clear();

		// -- Step finished
		LOG(INFO) << " -- Step finished in: " << (float)( clock() - step_start ) / CLOCKS_PER_SEC << "sec.";

		// -- Debug
		if (step % 20 == 0){
			cout << "Starting step #" << step << endl;
			if (pool.size() >= 5){
				cout << "Top 5 reliabilities: ";
				for (uint i = 0; i < 5; i++){
					cout << NetworkTest::getReliability(sorted_pool[i], tests) << ", ";
				}
				cout << endl;
				cout << "Top 5 advantages: ";
				for (uint i = 0; i < 5; i++){
					cout << NetworkTest::getAdvantage(sorted_pool[i], tests) << ", ";
				}
				cout << endl;
			}
		}

	}

#ifdef CUDA_ENABLED
	// -- Releasing memory for CUDA device
	//
	if (is_CUDA()){
		boost::static_pointer_cast<CUDATutorAlgorithms>(algos)->free();
	}
#endif

	LOG(INFO) << "Education finished";
}		/* -----  end of method Tutor::educate  ----- */

