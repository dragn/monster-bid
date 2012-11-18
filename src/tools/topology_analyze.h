/*
 * =====================================================================================
 *
 *       Filename:  topology_analyzer.h
 *
 *    Description:  Neural network topology analyzing tool
 *
 *        Version:  1.0
 *        Created:  12/05/2011 02:12:55 AM
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
#include "boost/shared_array.hpp"

namespace mb {

/*
 * =====================================================================================
 *        Class:  TopologyAnalyze
 *  Description:  
 * =====================================================================================
 */
class TopologyAnalyze
{
	public:
	~TopologyAnalyze () {};                            /* destructor       */
	
	/*
	 *--------------------------------------------------------------------------------------
	 *       Class:  TopologyAnalyze
	 *      Method:  TopologyAnalyze :: analyze_and_get_depth
	 * Description:  returns the depth of the defined topology, i.e. a number of steps
	 * 				 needed to reach output layers. This method also checks that
	 * 				 all output nodes do get reached and are reached simultaneously, otherwise a warning
	 * 				 will be generated and the method will return -1.
	 *--------------------------------------------------------------------------------------
	 */
	static int analyze_and_get_depth(Topology* tp);

	private:
	TopologyAnalyze () {};                             /* constructor      */

}; /* -----  end of class TopologyAnalyze  ----- */

}
