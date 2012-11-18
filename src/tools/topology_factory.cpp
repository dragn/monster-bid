/*
 * =====================================================================================
 *
 *       Filename:  topology_factory.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/06/2011 10:27:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "topology_factory.h"
#include "topology_analyze.h"

using namespace mb;

/*
 *--------------------------------------------------------------------------------------
 *       Class:  TopologyFactory
 *      Method:  TopologyFactory
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
TopologyFactory::TopologyFactory ()
{
}  /* -----  end of method TopologyFactory::TopologyFactory  (constructor)  ----- */


boost::shared_ptr<Topology>
TopologyFactory::create_connected_layers_topology( 
	uint input_size, uint output_size, std::vector<uint> layers)
{
	uint size = input_size + output_size;
	uint i;

	std::vector<uint> all;
	all.push_back( input_size );
	foreach (i, layers){
		size += i;
		all.push_back( i );
	}
	all.push_back( output_size );
	boost::shared_ptr<Topology> tp( new Topology( size, input_size, output_size ) ); 

	assert( all.size() > 1 );

	uint cursor = 0;
	for(uint layer = 0; layer < all.size() - 1; layer++){
		uint current = all[layer];
		uint next = all[layer + 1];
		for (uint i = cursor; i < cursor + current; i++){
			for (uint j = cursor + current; j < cursor + current + next; j++){
				tp->add_connection(i, j);
			}
		}	
		cursor += current;
	}

	assert( TopologyAnalyze::analyze_and_get_depth( tp.get() ) == (int) all.size() );

	return tp;
}		/* -----  end of method TopologyFactory::create_connected_layers_topolog  ----- */


