/*
 * ArrayFiller.hpp
 *
 *  Created on: Oct 1, 2016
 *      Author: Sina M.Baharlou
 *      The class for filling 1-D boolean array
 */

#ifndef ARRAYFILLER_HPP_
#define ARRAYFILLER_HPP_

#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <set>
#include <fstream>
#include <random>
#include <math.h>
#include "FillMethod.hpp"

#define DEFAULT_STEP_SIZE	0.05
#define DEFAULT_CURR_STEP	0.05

using namespace std;

class ArrayFiller
{
private :
	bool *bArray ;					// -- pointer to the boolean array
	unsigned int arrSize;			// -- array size
	float stepSize;					// -- STEP_FILL, step size
	float currStep;					// -- STEP_FILL, current step
	float tRate;					// -- PORTION_FILL, true-rate
	float* pArray;					// -- PORTION_EL_FILL array

	std::uniform_real_distribution<float> rndReal;	// -- sampling from real uniform  distribution
	std::default_random_engine rndEngine;			// -- sampling engine


public:

	// -- Vector Filler constructor
	ArrayFiller(bool* bArray,unsigned int arrSize);

	// -- property methods --
	bool* getArray();

	void setPortionArray(float* pArray);

	void setPortionRate(float rate);

	// -- Fill array randomly with the given method --
	bool* fillArray(FillMethod method);

};


#endif /* ARRAYFILLER_HPP_ */
