/*
 * TopologyDefinition.h
 *
 *  Created on: Oct 12, 2011
 *      Author: dragn
 */

#ifndef TOPOLOGY_H_
#define TOPOLOGY_H_

#include <list>
#include <string>
#include <iostream>
#include <algorithm>

#include "common/common_include.h"

namespace mb {

	/*
	 * =====================================================================================
	 *        Class:  Topology
	 *  Description:  Specifies topology of a network. Topology specified by:
	 *  			  input neurons count, output neurons count, total number
	 *  			  of neurons and a list of connections that exist between them.  
	 *
	 *  			  Perceptrons are enumerated from 0 to <total_size>, first <input_size>
	 *  			  of them are input neurons, last <output_size> of them are output neurons.
	 *
	 *  			  Basically, topology must be able to tell if there is a connection 
	 *  			  between perceptrons A and B.
	 * =====================================================================================
	 */
	class Topology
	{
		public:
			/* ====================  LIFECYCLE     ======================================= */

			Topology ();                             /* constructor      */
			Topology ( int total_size, int input_size, int output_size );
			Topology ( const Topology &other );   /* copy constructor */
			~Topology ();                            /* destructor       */

			/* ====================  ACCESSORS     ======================================= */

			int get_input_size();
			int get_output_size();
			int get_total_size();
			bool is_connected(int i, int j);

			/* ====================  MUTATORS      ======================================= */

			void add_connection(int i, int j);
			void remove_connection(int i, int j);

			/* ====================  OPERATORS     ======================================= */

			Topology& operator = ( const Topology &other ); /* assignment operator */

		protected:
			/* ====================  DATA MEMBERS  ======================================= */

		private:
			/* ====================  DATA MEMBERS  ======================================= */
			int total_size;
			int input_size;
			int output_size;

			std::list<int> connections;
			int get_connection_index( int, int );

	}; /* -----  end of class Topology  ----- */

}



#endif /* TOPOLOGY_H_ */
