/*
 * common_include.hpp
 *
 *  Created on: Nov 19, 2011
 *      Author: dragn
 */

#ifndef COMMON_INCLUDE_HPP_
#define COMMON_INCLUDE_HPP_

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_array.hpp>

typedef float t_weight;
typedef float t_data;

typedef unsigned int uint;

#ifdef __CDT_PARSER__
#define foreach(a, b) for (a : b)
#else
#define foreach(a, b) BOOST_FOREACH(a, b)
#endif

/* Logging mechanism */
#include <glog/logging.h>

#endif /* COMMON_INCLUDE_HPP_ */
