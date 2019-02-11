/*
 * HeuristicMethod.hpp
 *
 *  Created on: Oct 3, 2016
 *      Author: sina
 */

#ifndef HEURISTICMETHOD_HPP_
#define HEURISTICMETHOD_HPP_


enum HeuristicMethod
{
	FIRST_BEST,
	RND_BEST,
	RND_DIST,
	RND,
	RND_GREEDY,
	WALK_SAT,
	GSAT_WSAT,

};

#define FIRST_H_METHOD 	FIRST_BEST
#define LAST_H_METHOD 	GSAT_WSAT




#endif /* HEURISTICMETHOD_HPP_ */
