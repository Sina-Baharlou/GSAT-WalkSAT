/*
 * CFormula.hpp
 *
 *  Created on: Oct 1, 2016
 *      Author: Sina M.Baharlou
 */

#ifndef CFORMULA_HPP_
#define CFORMULA_HPP_

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
#include "ArrayFiller.hpp"
#include "HeuristicMethod.hpp"

using namespace std;


typedef int Literal;
typedef std::vector<Literal> Clause;
typedef std::vector<Clause> Formulae;



#define COMMENT_FLAG 'c'
#define PROBLEM_FLAG 'p'
#define END_FLAG '%'
#define toggle(x) (x=x^1)
#define DEFAULT_BIAS 1
#define DEFAULT_ITERATIONS 1000
#define DEFAULT_MAX_TRY 255
#define DEFAULT_GREEDY_FACTOR 0.8
#define DEFAULT_GW_FACTOR 0.5

class CFormula
{
public :
	string fType;			// -- formula type (in this case should be 'cnf')
	string fComments;		// -- formula comments
	unsigned int nVars;		// -- number of variables
	unsigned int nClauses;	// -- number of clauses
	struct
	{
		unsigned int nPosLits;		// -- number of positive literals
		unsigned int nNegLits;		// -- number of negative literals
		unsigned int nLits;			// -- total literls
	}fStats;

public :
	bool * fVars;				// -- formula variables
	float * fPortions;			// -- variables' proportions
	float tPortion;				// -- total positive to negative literal proportion
	int * nLit	;				// -- total occurrence of specific literal in formula
	int * evalArray;			// -- flip evaluation array
	bool * evalClause;		    // -- evaluation of clauses  // ADDED
	std::vector<int>* varMap;	// -- mapping between variables and clauses //ADDED
	unsigned int clCounter;		// -- clause counter variable
	float greedyFactor;			// -- GSAT greedy factor
	float gwFactor;				// -- GSAT or WSAT select probability

	Formulae cFormula;				// -- canonical formula jagged array(vector)
	ArrayFiller* arrayFiller;		//-- array filler class
	std::default_random_engine rndEngine;
	std::uniform_real_distribution<float> rndReal;	// -- sampling from real uniform  distribution
	std::uniform_int_distribution<int> rndMethod;   // -- sampling from int uniform  distribution
	std::uniform_int_distribution<int> rndHeuristic;// -- sampling from int uniform  distribution


public:
	CFormula();

	bool initFormula(const char* filename);

	bool evalFormulaQ(unsigned int varIndex,int& clNum);	// -- faster version of evalFormula (just check the modified clauses)

	bool evalFormula(int& clNum);

	bool startGSAT(int iterations=DEFAULT_ITERATIONS,int maxTry=DEFAULT_MAX_TRY,int fillMethod=-1,int hMethod=-1);

	void setGreedyFactor(float factor);

	void setGWFactor(float factor);



private:
	std::vector<string> tokenize(char*input,char*split);

	Clause parseClause(char*input,char*split);

	// -- randomly select the filler method --
	FillMethod getRandomMethod();

	// -- randomly select the heuristic method --
	HeuristicMethod getRandomHeuristic();

	// -- If GSAT should take a best action
	bool takeBestChoice();

	bool takeGW();

	void evalMove(int&min,int&max);

	std::vector<int> evalUnsatClause(int clIndex,int&min,int&max);

	int getBestChoice(bool retFirst=false);

	int walkSatChoice(bool retFirst=false);

	int getUnsatClause(bool retFirst=false);

	int getRndBest(bool exponential=true);	// -- randomly select the choice according to each move's reward


};


#endif /* CFORMULA_HPP_ */
