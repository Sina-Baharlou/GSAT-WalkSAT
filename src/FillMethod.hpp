/*
 * FillMethod.hpp
 *
 *  Created on: Oct 1, 2016
 *      Author: Sina M.Baharlou
 *      The methods for filling 1-D boolean array
 */

#ifndef FILLMETHOD_HPP_
#define FILLMETHOD_HPP_


enum FillMethod
{
	ZEROS_FILL,
	ONES_FILL,
	UNIFORM_FILL,
	STEP_FILL,
	PORTION_FILL,
	PORTION_EL_FILL
};

#define METHOD_COUNT 6
#define FIRST_FILL_METHOD 	UNIFORM_FILL
#define LAST_FILL_METHOD 	PORTION_EL_FILL
#define UNIFORM_RATIO	0.5

#endif /* FILLMETHOD_HPP_ */
