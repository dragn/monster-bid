/*
 * =====================================================================================
 *
 *       Filename:  cuda_tutor_algorithms.cu
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/14/2012 10:22:58 PM
 *       Revision:  none
 *       Compiler:  nvcc
 *
 *         Author:  daniel.sabelnikov (), dsabelnikov@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "cuda_tutor_algorithms.h"

using namespace mb;


/* =====================================================================================
 *    CUDA Device code
 * =====================================================================================*/

__device__ float _sigmoid(float in){
	return 1.0 / ( 1.0 + expf( -in ) );	
	//	return 1;
}

__device__ void _run_network( int network_size, int output_size, 
		float* weights, float* bias, size_t weights_pitch, float* map, float* map1) {
	bool stop = false;
	while (!stop){	
		stop = true;
		float* w_ptr;
		// -- process signal flows
		for (uint i = 0; i < network_size; i++){
			if (map[i] != 0){
				for (uint j = 0; j < network_size; j++){
					w_ptr = (float*)((char*)weights + weights_pitch * j);
					float w = w_ptr[i];
					if (w != 0){
						map1[j] += map[i] * w;
						stop = false;
					}
				}
				if (!stop) map[i] = 0;
			}
		}
		// apply sigmas
		for (uint i = 0; i < network_size; i++){
			if (map1[i] != 0){
				map[i] = _sigmoid(map1[i] + bias[i]);
				map1[i] = 0;
			}
		}	
	}
}

__device__ float _fitness( int output_size, float* net_output, float* test_output){
	float fit = 0;
	float delta = 0;
	for (int i = 0; i < output_size; i++){
		delta = ( net_output[i] / test_output[i] - 1);
		fit += delta * delta;
	}
	return fit;
}

__device__ float _fitness_alpha( int output_size, float* net_output, float* test_output){
	float score = 0;
	if ( output_size != 2 ) return 0;
	// | |result| - |test| | + | |intersect| - |result| |
/*	if ( net_output[1] < net_output[0]){
		// -- first value greater than second
		//
		score = 10;
	} else {
		float z1 = 0;
		float z2 = 0;
		float x1 = test_output[0];
		float x2 = test_output[1];
		float y1 = net_output[0];
		float y2 = net_output[1];
		if ( x1 <= y1 && y1 <= x2 ){
			z1 = y1;
		} else if ( y1 <= x1 && x1 <= y2 ) {
			z1 = x1;
		}
		if ( x1 <= y2 && y2 <= x2 ){
			z2 = y2;
		} else if ( y1 <= x2 && x2 <= y2 ) {
			z2 = x2;
		}
		float delta = ( fabs( (x2 - x1) - (y2 - y1) ) + fabs( (z2 - z1) - (y2 - y1) ) ) / (y2 - y1);
		score = delta * delta;
	}*/
	float fit = 0;
	float delta = 0;
	float x1 = test_output[0];
	float x2 = test_output[1];
	float y1 = net_output[0];
	float y2 = net_output[1];

	delta = (y1 - x1) * (y1 - x1);
	if (y1 < x1) delta *= delta;
	fit += delta;

	delta = (y2 - x2) * (y2 - x2);
	if (y2 > x2) delta *= delta;
	fit += delta;

	return fit;
}

__global__ void ExaminationKernel(
		cudaPitchedPtr d_networks, 
		float* d_biases, size_t d_biases_pitch,
		float* d_test_input, size_t d_test_input_pitch, 
		float* d_test_output, size_t d_test_output_pitch, 
		float* d_calc, size_t d_calc_pitch, 
		float* d_scores, 
		const int network_size, const int input_size,
		const int output_size, const int tests_size, const int network_type){

	// -- This thread's index - index of associated network
	int net_index = threadIdx.x;

	// -- Pointer to weight map of associated network
	float* weight_ptr = (float*)( (char*)d_networks.ptr + d_networks.pitch * network_size * net_index );

	// -- pointer to bias map
	float* bias_ptr = (float*)( (char*)d_biases + d_biases_pitch * net_index );

	// -- This thread's calculation area
	float* calc1_ptr = (float*)( (char*) d_calc + d_calc_pitch * (2 * net_index) ); 
	float* calc2_ptr = (float*)( (char*) d_calc + d_calc_pitch * (2 * net_index + 1) );

/*	if (threadIdx.x == 1){
		for (uint x = 0; x < network_size; x++){
			float* row = (float*) ((char*)weights_ptr + d_networks.pitch * x);
			for (uint y = 0; y < network_size; y++){
				buffer[y * network_size + x] = row[y];
			}
		}
	}*/

	// -- Where to put scores
	float* scores_ptr = d_scores + net_index;

	*scores_ptr = 0;

	// -- tests cycle
	for (int test = 0; test < tests_size; test++){
		// -- pointers to input and output data
		float* input_ptr = (float*)( (char*)d_test_input + d_test_input_pitch * test );
		float* output_ptr = (float*)( (char*)d_test_output + d_test_output_pitch * test );

		// -- clear the buffers
		for (int i = 0; i < network_size; i++) { calc1_ptr[i] = 0; calc2_ptr[i] = 0; }

		// -- put input data
		for (int i = 0; i < input_size; i++){
			calc1_ptr[i] = input_ptr[i];
		}	

		// -- run network
		_run_network( network_size, output_size, weight_ptr, bias_ptr, d_networks.pitch, calc1_ptr, calc2_ptr );
		float* net_output_ptr = calc1_ptr + (network_size - output_size);

		// -- calculate and append score
		if (network_type == 0){
			*scores_ptr += _fitness(output_size, net_output_ptr, output_ptr);
		}else if (network_type == 1){
			*scores_ptr += _fitness_alpha(output_size, net_output_ptr, output_ptr);
		}
		//		*scores_ptr = 1;
	}

	*scores_ptr = sqrtf( *scores_ptr ) / tests_size;
}

/* =====================================================================================
 *    End of CUDA Device code
 * =====================================================================================*/

CUDATutorAlgorithms::CUDATutorAlgorithms() {	
	allocated = false;
}		/* -----  end of constructor CUDATutorAlgorithm           ----- */

std::vector<float> 
CUDATutorAlgorithms::examination(std::vector<NetworkPtr> pool, std::vector<TestCasePtr> tests)
{
	std::vector<float> scores;
	if (pool.size() == 0 || tests.size() == 0){
		return scores; // Return empty array if no networks or no tests.
	}
	// -- Allocate memory on device (if it's not already allocated)
	//
	int network_size = pool[0]->get_size();
	int input_size = pool[0]->get_input_size();
	int output_size = pool[0]->get_output_size();
	allocate(network_size, input_size, output_size, pool.size(), tests.size());

	// -- Copy networks' weights and biases to device
	//
	size_t pitch = d_networks.pitch;
	for (uint i = 0; i < pool.size(); i++){
		char* w_ptr = (char*) d_networks.ptr + pitch * network_size * i;
		cudaMemcpy2D( w_ptr, pitch, pool[i]->get_weight_map(), network_size * sizeof(float), 
				network_size * sizeof(float), network_size, cudaMemcpyHostToDevice );
		char* b_ptr = (char*) d_biases + d_biases_pitch * i;
		cudaMemcpy( b_ptr, pool[i]->get_bias_map(), network_size * sizeof(float), 
				cudaMemcpyHostToDevice);
	}

	// -- copy tests to device
	//
	for (uint i = 0; i < tests.size(); i++){
		char* input_ptr = (char*) d_test_input + d_test_input_pitch * i;
		char* output_ptr = (char*) d_test_output + d_test_output_pitch * i;
		cudaMemcpy(input_ptr, tests[i]->get_input().get(), input_size * sizeof(float),
				cudaMemcpyHostToDevice);
		cudaMemcpy(output_ptr, tests[i]->get_output().get(), output_size * sizeof(float),
				cudaMemcpyHostToDevice);
	}

	// -- clearing temp & scores memory
	//
	cudaMemset2D(d_calc, d_calc_pitch, 0, network_size * sizeof(float), pool.size());
	cudaMemset(d_scores, 0, pool.size() * sizeof(float));

	boost::shared_array<float> buffer( new float[network_size*network_size] );
	float* d_buffer;
	cudaMalloc( &d_buffer, network_size * network_size * sizeof(float) );

	// -- run kernel
	//
	ExaminationKernel<<<1, pool.size()>>>(d_networks, d_biases, d_biases_pitch, d_test_input,
			d_test_input_pitch, d_test_output, d_test_output_pitch,
			d_calc, d_calc_pitch, d_scores, network_size, input_size,
			output_size, tests.size(), network_type == "alpha" ? 1 : 0 );

	cudaThreadSynchronize();

	cudaMemcpy(buffer.get(), d_buffer, network_size * network_size * sizeof(float), cudaMemcpyDeviceToHost);
//	LOG(INFO) << "Buffer state: ";
	/*for (uint i = 0; i < network_size; i++){
		for (uint j = 0; j < network_size; j++){
			LOG(INFO) << "  " << buffer[i*network_size + j];
		}
		LOG(INFO) << "  ---";
	}*/
	cudaFree( d_buffer );

	// -- copy scores from device
	//
	boost::shared_array<float> temp_scores( new float[pool.size()] );
	cudaMemcpy(temp_scores.get(), d_scores, pool.size() * sizeof(float), cudaMemcpyDeviceToHost); 
	for (uint i = 0; i < pool.size(); i++){
		scores.push_back( temp_scores[i] );	
	}
	return scores;
}		/* -----  end of method CUDATutorAlgorithms::examination  ----- */

void 
CUDATutorAlgorithms::free() {
	if (allocated) {
		// do free
		cudaFree( d_test_input );
		cudaFree( d_test_output );
		cudaFree( d_networks.ptr );
		cudaFree( d_biases );
		cudaFree( d_calc );
		cudaFree( d_scores );

		size_t free;
		size_t total;
		cudaMemGetInfo(&free, &total);
		LOG(INFO) << "Device memory released. Memory usage: " << total - free << "/" << total;

		allocated = false;
	}
}		/* -----  end of method CUDATutorAlgorithms::free  ----- */

void 
CUDATutorAlgorithms::allocate(int network_size, int input_size, int output_size, int pool_size, int tests_size) {
	if (!allocated){
		// do allocate
		cudaMallocPitch( &d_test_input, &d_test_input_pitch, input_size *sizeof(float), tests_size );
		cudaMallocPitch( &d_test_output, &d_test_output_pitch, output_size *sizeof(float), tests_size );

		cudaExtent extent = make_cudaExtent( network_size * sizeof(float), network_size, pool_size );
		cudaMalloc3D( &d_networks, extent );

		cudaMallocPitch( &d_biases, &d_biases_pitch, network_size * sizeof(float), pool_size );

		cudaMallocPitch( &d_calc, &d_calc_pitch, network_size * sizeof(float), 2 * pool_size );

		cudaMalloc( &d_scores, pool_size * sizeof(float) );

		size_t free;
		size_t total;
		cudaMemGetInfo(&free, &total);
		LOG(INFO) << "Device memory allocated. Memory usage: " << total - free << "/" << total;

		allocated = true;
	} 
}		/* -----  end of method CUDATutorAlgorithms::allocate  ----- */


