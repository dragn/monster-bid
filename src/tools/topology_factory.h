/*
 * =====================================================================================
 *
 *       Filename:  topology_factory.h
 *
 *    Description:  Topology factory
 *
 *        Version:  1.0
 *        Created:  12/06/2011 10:20:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Sabelnikov Daniil (), dsabelnikov@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "common/common_include.h"
#include "network/topology.h"

#include <vector>
#include <boost/shared_ptr.hpp>

namespace mb {

	/*
	 * =====================================================================================
	 *        Class:  TopologyFactory
	 *  Description:  
	 * =====================================================================================
	 */
	class TopologyFactory
	{
		public:
			/* ====================  OPERATORS     ======================================= */

			
			/*
			 *--------------------------------------------------------------------------------------
			 *       Class:  TopologyFactory
			 *      Method:  TopologyFactory :: create_connected_layers_topology
			 * Description:  Generates a topology with fully-connected layers.
			 * 				 Returns pointer to the generated topology.
			 *--------------------------------------------------------------------------------------
			 */
			static boost::shared_ptr<Topology> create_connected_layers_topology(
					uint input_size, uint output_size, std::vector<uint> layers);

		private:
			TopologyFactory ();                             /* constructor */

	}; /* -----  end of class TopologyFactory  ----- */


}
