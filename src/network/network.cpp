/*
 * =====================================================================================
 *
 *       Filename:  network.cpp
 *
 *    Description:  Network class implementatin
 *
 *        Version:  1.0
 *        Created:  12/03/2011 01:14:40 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  :
 *
 * =====================================================================================
 */

#include "network.h"

using namespace mb;

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Network
 *      Method:  Network
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Network::Network(Topology* tp, t_weight max_weight, t_weight max_bias)
{
	int depth = TopologyAnalyze::analyze_and_get_depth(tp);
	if (depth < 0){
		LOG(FATAL) << "Topology is invalid! Network cannot be created!";
	}

	total_size = tp->get_total_size();
	input_size = tp->get_input_size();
	output_size = tp->get_output_size();

	assert ( total_size > 0 );
	assert ( input_size > 0 );
	assert ( output_size > 0 );

	if (max_weight <= 0){
		LOG(FATAL) << "Network creation failed: max_weight must be a positive number";
	}
	if (max_bias < 0){
		LOG(FATAL) << "Network creation failed: max_bias must be a non-negative number";
	}

	init_data();

	/* Generate weights */
	for (uint i = 0; i < total_size; i++){
		for (uint j = 0; j < total_size; j++){
			if (tp->is_connected(i, j)){
				t_weight rand_weight = max_weight * ( 2 * ( (t_weight) rand() / RAND_MAX ) - 1 );
				set_weight(i, j, rand_weight);
			}
		}
	}

	/* Generate biases */
	for (uint i = 0; i < total_size; i++){
		set_bias( i, max_bias * ((t_weight) rand() / RAND_MAX ) ); 
	}
}  /* -----  end of method Network::Network  (constructor)  ----- */


/*
 *--------------------------------------------------------------------------------------
 *       Class:  Network
 *      Method:  Network
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Network::Network (uint size, uint input_size, uint output_size, t_weight *weight_map, t_weight *bias_map)
{
	total_size = size;
	this->input_size = input_size;
	this->output_size = output_size;
	init_data();
	for (uint i = 0; i < size; i++){
		for (uint j = 0; j < size; j++){
			set_weight( i, j, weight_map[ j * size + i] );
		}
	}
	for (uint i = 0; i < size; i++){
		set_bias( i, bias_map[ i ] );
	}
}  /* -----  end of method Network::Network  (constructor)  ----- */


/*
 *--------------------------------------------------------------------------------------
 *       Class:  Network
 *      Method:  Network
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Network::Network (std::istream* is)
{
	*is >> total_size;
	*is >> input_size;
	*is >> output_size;
	init_data();

	t_weight w;
	for (uint i = 0; i < total_size; i++){
		for (uint j = 0; j < total_size; j++){
			*is >> w;
			set_weight( i, j, w);
		}
	}
	for (uint i = 0; i < total_size; i++){
		*is >> w;
		set_bias(i, w);
	}	
}  /* -----  end of method Network::Network  (constructor)  ----- */

Network::Network () {
}
Network::~Network () {}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Network
 *      Method:  Network :: get_weight
 * Description:  returns weight of connection between i's and j's perceptron
 *--------------------------------------------------------------------------------------
 */
t_weight
Network::get_weight ( uint i, uint j )
{
	if (i >= 0 && j >= 0 && i < total_size && j < total_size){
    	return weight_map[j * total_size + i];
	}else{
		LOG(WARNING) << "Invalid indeces: [" << i << ", " << j << "]";
		return 0;
	}
}		/* -----  end of method Network::get_weight  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Network
 *      Method:  Network :: get_bias
 * Description:  returns bias of i's perceptron
 *--------------------------------------------------------------------------------------
 */
t_weight
Network::get_bias ( uint i )
{
	if (i >= 0 && i < total_size){
    	return weight_map[total_size * total_size + i];
	}else{		
		LOG(WARNING) << "Invalid index: " << i;
		return 0;
	}
}		/* -----  end of method Network::get_bias  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Network
 *      Method:  Network :: print
 * Description:  prints a "human-readable" weights table to stream
 *--------------------------------------------------------------------------------------
 */
void 
Network::print(std::ostream* os){
	*os << "Generic perceptrons network." << std::endl;
	*os << "  Total size: " << total_size << std::endl;
	*os << "  Input size: " << input_size << std::endl;
	*os << "  Output size: " << output_size << std::endl;
	*os << "  Wheight map: " << std::endl;
	os->precision(3);
	os->setf(std::ios::fixed, std::ios::floatfield);
	for (uint j = 0; j < total_size; j++){
		*os << "\t";
		for (uint i = 0; i < total_size; i++){
			t_weight w = get_weight(i, j);
			if (w != 0){
				*os << w << " \t";
			}else{
				*os << "-      \t";
			}
		}	
		*os << std::endl;
	}
	*os << "Bias:\t";
	for (uint i = 0; i < total_size; i++){			
		*os << get_bias(i) << " \t";
	}
}


/*
 *--------------------------------------------------------------------------------------
 *       Class:  Network
 *      Method:  Network :: serialize
 * Description:  serializes network's data to output stream
 *--------------------------------------------------------------------------------------
 */
void 
Network::serialize(std::ostream* os){
	*os << std::scientific;
	*os << std::setprecision(20);
	*os << total_size << " " << input_size << " " << output_size << " ";
	for (uint i = 0; i < total_size; i++){
		for (uint j = 0; j < total_size; j++){
			*os << get_weight(i, j) << " ";
		}
	}
	for (uint i = 0; i < total_size; i++){
		*os << get_bias(i) << " ";
	}
}


void Network::set_weight ( uint i, uint j, t_weight weight )
{
	weight_map[j * total_size + i] = weight;
}		/* -----  end of method Network::set_weight  ----- */


void Network::set_bias ( uint i, t_weight bias )
{
	weight_map[total_size * total_size + i] = bias;
}		/* -----  end of method Network::set_bias  ----- */


void Network::init_data ()
{
	weight_map.reset( new t_weight[ total_size * (total_size + 1) ] );
	memset( weight_map.get(), 0, total_size * (total_size + 1) * sizeof(t_weight) ); 
}		/* -----  end of method Network::init_data  ----- */


uint Network::get_input_size ()
{
	return input_size;
}		/* -----  end of method Network::get_input_size  ----- */


uint Network::get_output_size ()
{
	return output_size;
}		/* -----  end of method Network::get_output_size  ----- */


uint Network::get_size ()
{
	return total_size;
}		/* -----  end of method Network::get_size  ----- */

