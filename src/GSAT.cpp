//============================================================================
// Name        : GSAT.cpp
// Author      : Sina M.Baharlou
// Version     : 1
// Description : GSAT Solver
//============================================================================



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
#include <sys/time.h>

#include "FillMethod.hpp"
#include "ArrayFiller.hpp"
#include "CFormula.hpp"
using namespace std;



using namespace std ;


#include <algorithm>

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
	char ** itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
	return std::find(begin, end, option) != end;
}

void printHelp()
{
	char *help = "\n\n// --  GSAT simple solver -- // \n"
			"// --  By Sina M.Baharlou -- // \n"
			"// --  Oct  2016 \n\n"
			"GSAT -i [input_file] <options> \n"
			"Available options are : \n"
			"\t--iterations [number of iterations] \n"
			"\t--flip_max [number of maximum flips per iterations]\n"
			"\t--greedy_factor [greedy factor] \n"
			"\t--gw_factor [gsat/wsat factor] \n"
			"\t--heuristic [heuristic_method]\n"
			"\t\t-1- randomly select the heuristic method\n"
			"\t\t 0- select the first best choice\n"
			"\t\t 1- select randomly among the best choices\n"
			"\t\t 2- select according to the cost distribution\n"
			"\t\t 3- select randomly among all the choices\n"
			"\t\t 4- combination of 1 and 2 according to the greedy factor\n"
			"\t\t 5- WALKSAT\n"
			"\t\t 6- randomly select between GSAT or WALKSAT\n"
			"\t--rnd_method [rnd_method] \n"
			"\t\t-1-randomly select the method\n"
			"\t\t0-fill with zeros\n"
			"\t\t1-fill with ones\n"
			"\t\t2-fill uniformly\n"
			"\t\t3-STEP method\n"
			"\t\t4-PORTION method\n"
			"\t\t5-PORTION_EL method\n";


	printf(help);
}


#define DEFAULT_ITERATIONS 1000
#define DEFAULT_MAX_TRY 255
#define DEFAULT_GREEDY_FACTOR 0.8
#define DEFAULT_GW_FACTOR 0.5
#define DEFAULT_RND_METHOD -1
#define DEFAULT_H_METHOD -1

int main(int argc,char* argv[])
{

	// -- default variables --
	int iterations=DEFAULT_ITERATIONS;
	int maxTry=DEFAULT_MAX_TRY;
	float greedyFactor=DEFAULT_GREEDY_FACTOR;
	float gwFactor=DEFAULT_GW_FACTOR;
	int rndMethod=DEFAULT_RND_METHOD;
	int hMethod=DEFAULT_H_METHOD;
	// --


	if (argc<2)
	{
		printHelp();
		exit(0);
	}

	if (!cmdOptionExists(argv, argv+argc, "-i"))
	{
		printHelp();
		printf("STD Error : no input file has been provided.\n\n");
		exit(0);
	}

	char * filename = getCmdOption(argv, argv + argc, "-i");

	if (filename==NULL)
	{
		printf("STD Error : not input file has been provided.\n\n");
		exit(0);
	}

	if (cmdOptionExists(argv, argv+argc, "--iterations"))
		iterations=stoi(getCmdOption(argv, argv + argc, "--iterations"));



	if (cmdOptionExists(argv, argv+argc, "--flip_max"))
		maxTry=stoi(getCmdOption(argv, argv + argc, "--flip_max"));


	if (cmdOptionExists(argv, argv+argc, "--greedy_factor"))
		greedyFactor=stof(getCmdOption(argv, argv + argc, "--greedy_factor"));

	if (cmdOptionExists(argv, argv+argc, "--gw_factor"))
		gwFactor=stof(getCmdOption(argv, argv + argc, "--gw_factor"));

	if (cmdOptionExists(argv, argv+argc, "--rnd_method"))
		rndMethod=stoi(getCmdOption(argv, argv + argc, "--rnd_method"));

	if (cmdOptionExists(argv, argv+argc, "--heuristic"))
		hMethod=stoi(getCmdOption(argv, argv + argc, "--heuristic"));




	printf("[GSAT, filename=%s , iterations:%d , max try:%d , greedy factor :%f , rnd method :%d , heuristic method :%d]\n",
			filename,iterations,maxTry,greedyFactor,rndMethod,hMethod);


	struct timeval start, end;
	long mtime, seconds, useconds;
	gettimeofday(&start, NULL);

	// -- GSAT --
	CFormula* formula=new CFormula();
	formula->initFormula(filename);
	formula->setGreedyFactor(greedyFactor);
	formula->setGWFactor(gwFactor);
	bool eval=formula->startGSAT(iterations,maxTry,rndMethod,hMethod);

	// --

	gettimeofday(&end, NULL);
	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;


	printf("Elapsed time: %ld milliseconds\n", mtime);


	return eval;
}
