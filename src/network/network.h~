/*
 * Network.h
 *
 *  Created on: Feb 24, 2011
 *      Author: dragn
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <iomanip>

#include "boost/shared_array.hpp"

#include "common/common_include.h"
#include "topology.h"
#include "tools/topology_analyze.h"

namespace mb {

	/**
	 * Generic neural Network class.
	 * Object Network represents a single neural Network specified
	 * with some _topology_ and defined by perceptrons' weights.
	 */
	class Network {
		public:
			// Constructor must build a random-weight Network
			Network(Topology* tp, t_weight max_weight, t_weight max_bias);

			// Construct a network with defined weight and bias maps
			Network(uint size, uint input_size, uint output_size, t_weight *weight_map, t_weight *bias_map);

			// Creates new network instance by deserializing from input stream
			Network(std::istream*);

			Network();
			virtual ~Network();

			/**
			 *  Returns the connection weight between i's and j's perceptron
			 */
			t_weight get_weight(uint i, uint j);

			/**
			 *  Returns the bias of i's perceptron
			 */
			t_weight get_bias(uint i);

			/**
			 * Prints readable representation of this Network to specified stream.
			 */
			void print(std::ostream*);

			/**
			 * serialization/deserialization algorithms
			 */
			void serialize(std::ostream*);

			uint get_input_size();
			uint get_output_size();
			uint get_size();

			void set_weight(uint, uint, t_weight);
			void set_bias(uint, t_weight);

			t_weight* get_weight_map(){
				return weight_map.get();
			}

			t_weight* get_bias_map(){
				return weight_map.get() + total_size * total_size;
			}

		private:
			boost::shared_array<t_weight> weight_map;

			uint input_size;
			uint output_size;
			uint total_size;

			void init_data();
	};

	typedef boost::shared_ptr<Network> NetworkPtr;

}

#endif /* NETWORK_H_ */
