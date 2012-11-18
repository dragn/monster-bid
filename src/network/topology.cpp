/*
 * =====================================================================================
 *
 *       Filename:  topology.cpp
 *
 *    Description:  Topology class implementation
 *
 *        Version:  1.0
 *        Created:  12/03/2011 02:19:13 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "topology.h"

using namespace mb;

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Topology
 *      Method:  Topology
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Topology::Topology (): total_size(3), input_size(1), output_size(1)
{
}  /* -----  end of method Topology::Topology  (constructor)  ----- */

Topology::~Topology(){}

/*
 *--------------------------------------------------------------------------------------
 *       Class:  Topology
 *      Method:  Topology
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
Topology::Topology ( int total_size, int input_size, int output_size):
	total_size(total_size), input_size(input_size), output_size(output_size)
{
	if (total_size <= 0 || input_size <= 0 || output_size <= 0){
		LOG(FATAL) << "Total, input and output size must be positive";
	}
	if (total_size < input_size + output_size){
		LOG(WARNING) << "Invalid total_size for topology. Total size will be set to input_size + output_size ";
		this->total_size = input_size + output_size;
	}	
}  /* -----  end of method Topology::Topology  (constructor)  ----- */

int Topology::get_total_size ()
{
	return total_size;
}		/* -----  end of method Topology::get_total_size  ----- */

int Topology::get_output_size ()
{
	return output_size;
}		/* -----  end of method Topology::get_output_size  ----- */

int Topology::get_input_size ()
{
	return input_size;
}		/* -----  end of method Topology::get_input_size  ----- */


bool Topology::is_connected( int i, int j )
{
	int index = get_connection_index( i, j );
	return std::binary_search( connections.begin(), connections.end(), index ); 
}		/* -----  end of method Topology::is_connected  ----- */


void Topology::add_connection ( int i, int j )
{
	if (i < 0 || i > total_size || j < 0 || j > total_size){
		LOG(WARNING) << "Ignoring invalid indexes for new connection: " << i << ", " << j;
		return;
	}
	if (i < input_size && j < input_size){
		LOG(WARNING) << "Ignoring connection between input perceptrons: " << i << ", " << j;
		return;
	}
	if (i >= total_size - output_size && j >= total_size - output_size){
		LOG(WARNING) << "Ignoring connection between output perceptrons: " << i << ", " << j;
		return;
	}
	connections.push_back( get_connection_index( i, j ) );
	connections.sort();
	return ;
}		/* -----  end of method Topology::add_connection  ----- */

void Topology::remove_connection ( int i, int j )
{
	int index = get_connection_index( i, j );
	connections.remove( index );
	return ;
}		/* -----  end of method Topology::remove_connection  ----- */


inline int Topology::get_connection_index ( int i, int j )
{
	return j * total_size + i;
}		/* -----  end of method Topology::get_connection_index  ----- */

