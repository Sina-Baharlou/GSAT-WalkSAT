/*
 * CFormula.cpp
 *
 *  Created on: Oct 1, 2016
 *      Author: Sina M.Baharlou
 */

#include "CFormula.hpp"

CFormula::CFormula():
nVars(0),
nClauses(0),
fVars(NULL),
fPortions(NULL),
tPortion(0),
nLit(NULL),
evalArray(NULL),
fStats{0,0,0},
varMap(NULL),
evalClause(NULL),
clCounter(0),
arrayFiller(NULL),
greedyFactor(DEFAULT_GREEDY_FACTOR)
{

	this->fType.clear();
	this->fComments.clear();

	// -- Initializing distribution variables --

	rndReal=std::uniform_real_distribution<float>(0.0,1.0);
	rndMethod=std::uniform_int_distribution<int>(FIRST_FILL_METHOD,LAST_FILL_METHOD);
	rndHeuristic=std::uniform_int_distribution<int>(FIRST_H_METHOD,LAST_H_METHOD);

	// -- Init random engine --

	struct timespec ts;
	unsigned theTick = 0U;
	clock_gettime( CLOCK_REALTIME, &ts );
	theTick  = ts.tv_nsec / 1000000;
	theTick += ts.tv_sec * 1000;
	rndEngine.seed(theTick);
}




bool CFormula::initFormula(const char* filename)
{

	// -- Initialize variables --

	this->cFormula.empty();
	this->fComments.empty();
	this->fType.empty();
	this->fStats={0,0,0};
	this->clCounter=0;
	bool fReached=false;


	// -- Free previously allocated variables --

	if (this->fVars) free(this->fVars);
	if (this->fPortions) free(this->fPortions);
	if (this->nLit) free(this->nLit);
	if (this->evalArray) free(this->evalArray);
	if (this->arrayFiller) free(this->arrayFiller);
	if (this->varMap) free(this->varMap);
	if (this->evalClause) free(this->evalClause);

	// -- Open input file --

	ifstream input_file;
	input_file.open(filename);

	if (!input_file.is_open())
	{
		printf("STD Error: cannot open '%s'.",filename);
		return false;
	}


	// -- Parse the provided file --

	std::string line;
	while (std::getline(input_file, line))
	{
		// -- get the flag (can be 'c' or 'p') --
		char flag=line[0];
		std::vector<string> tokens;

		switch (flag)
		{

		case COMMENT_FLAG:	// -- Comment line has reached --

			// -- Add to comment string --
			this->fComments.append(&line[1]);
			this->fComments.append("\n");
			break;


		case PROBLEM_FLAG:	// -- Problem line has reached --

			// -- Tokenize current line --
			tokens.empty();
			tokens=tokenize(&line[1]," ");

			// -- Check if the syntax if correct in order to parse the parameters --
			if (tokens.size()<3)
			{
				printf("There is a syntax error in the problem definition.");
				return false;
			}

			// -- Assign the variables --
			this->fType=tokens[0];
			this->nVars=stoi(tokens[1])+1;		// -- starts from 1
			this->nClauses=stoi(tokens[2]);
			fReached=true;

			// -- allocate the arrays
			this->fVars=new bool[this->nVars];
			this->fPortions= new float[this->nVars];
			this->nLit=new int[this->nVars];
			this->evalArray=new int[this->nVars];
			this->evalClause=new bool[this->nClauses];
			this->varMap=new std::vector<int>[this->nVars];

			// -- initialize the arrays
			memset(this->fVars,0,this->nVars*sizeof(bool));
			memset(this->fPortions,0,this->nVars*sizeof(float));
			memset(this->nLit,0,this->nVars*sizeof(int));
			memset(this->evalArray,0,this->nVars*sizeof(int));
			memset(this->evalClause,0,this->nClauses*sizeof(bool));

			// -- Instantiate the arrayFiller class
			this->arrayFiller=new ArrayFiller(this->fVars,this->nVars);

			break;


		case END_FLAG: // -- End of the formula has reached --
			input_file.close();

			// -- Ppdate stats --
			this->tPortion=(float)this->fStats.nPosLits/this->fStats.nLits;
			for (int i=1;i<this->nVars;i++)
				this->fPortions[i]/=(float)this->nLit[i];

			// -- Init random filler --
			this->arrayFiller->setPortionRate(this->tPortion);
			this->arrayFiller->setPortionArray(this->fPortions);
			this->arrayFiller->fillArray(PORTION_EL_FILL);
			return true;
			break;

		default:	// -- Clause line has reached--
			if (!fReached)
				continue;
			this->cFormula.push_back(parseClause(&line[0]," "));
			break;
		}

	}

	return true;
}




bool CFormula::evalFormulaQ(unsigned int varIndex,int& clNum)
{

	bool fEval=true;
	clNum=0;

	// -- check through the modified clauses --
	for (int i=0;i<this->varMap[varIndex].size();i++)
	{
		// -- get the clause --
		int cIndex=this->varMap[varIndex].at(i);
		Clause cl=this->cFormula[cIndex];

		// -- for each literal
		for (int j=0;j<cl.size();j++)
		{

			Literal lit=cl[j];

			// -- get the index and sign
			bool neg=(lit<0);
			int index=abs(lit);
			// -- evaluate the literal
			bool litEval=neg^this->fVars[index];
			this->evalClause[cIndex]=litEval; // -- update clause truth list

			// -- if positive -> clause will be evaluated as positive
			if (litEval)
				break;

		}

	}


	// -- evaluate the whole formula --

	for (int i=0;i<this->cFormula.size();i++)
	{
		bool evalClause=this->evalClause[i];
		fEval&=evalClause;
		if (evalClause)clNum++;
	}
	return fEval;
}


bool CFormula::evalFormula(int& clNum)
{


	bool fEval=true;
	clNum=0;


	for (int i=0;i<this->cFormula.size();i++)
	{

		// -- get the clause --
		Clause cl=this->cFormula[i];
		bool clEval=false;

		// -- for each literal
		for (int j=0;j<cl.size();j++)
		{

			Literal lit=cl[j];

			// -- get the index and sign
			bool neg=(lit<0);
			int index=abs(lit);
			// -- evaluate the literal
			bool litEval=neg^this->fVars[index];

			this->evalClause[i]=litEval;

			// -- if positive -> clause will be evaluated as positive
			if (litEval)
			{
				clEval=true;
				clNum++;
				break;
			}
		}

		// -- and it with current evaluation of formula
		fEval&=clEval;

	}

	return fEval;
}



bool CFormula::startGSAT(int iterations,int maxTry,int fillMethod,int hMethod)
{
	float initGuessMean=0;	// -- initial guess average
	float negClauseMean=0;	// -- average number of negative clauses
	float elaborateMean=0; 	// -- averate number of elaboration
	bool fEval=false;
	int i;

	for (i=0;i<iterations;i++)
	{
		int initGuess=0;
		int clNum=0;
		fEval=false;

		// -- randomly select the filler method --
		FillMethod fMethod;
		if (fillMethod==-1)
			fMethod=getRandomMethod();
		else
			fMethod=(FillMethod)fillMethod;

		// -- randomly select the heuristic method --
		HeuristicMethod heuristic;
		if (hMethod==-1)
			heuristic=getRandomHeuristic();
		else
			heuristic=(HeuristicMethod)hMethod;


		this->arrayFiller->fillArray(fMethod);

		// -- evaluate the function
		fEval=this->evalFormula(clNum);
		initGuess=clNum;
		initGuessMean+=initGuess;

		// -- return if already found the solution
		if (fEval)
			break;


		for (int j=0;j<maxTry;j++)
		{
			int choice=0;

			// -- select the choice with the given heuristics --

			switch (heuristic)
			{

			case FIRST_BEST:
				choice=this->getBestChoice(true);
				break;

			case RND_BEST:
				choice=this->getBestChoice();
				break;

			case RND_DIST:
				choice=this->getRndBest();
				break;

			case RND:
				choice=(int)(rndReal(rndEngine)*this->nVars);
				break;

			case RND_GREEDY:
				if (this->takeBestChoice())
					choice=this->getBestChoice();
				else
					choice=this->getRndBest();
				break;

			case WALK_SAT:
				choice=this->walkSatChoice();
				break;

			case GSAT_WSAT:
				if (this->takeGW())
					choice=this->getBestChoice();
				else
					choice=this->walkSatChoice();
				break;

			}


			toggle(this->fVars[choice]);
			fEval=this->evalFormulaQ(choice,clNum);//this->evalFormula(clNum);

			if (fEval)
				break;


		}


		negClauseMean+=this->nClauses-clNum;
		elaborateMean+=clNum-initGuess;


		printf("[iteration %d, fill :%d, heuristic :%d] - init Guess :%d - final Guess :%d - model found :%d\n",i,fMethod,heuristic,initGuess,clNum,fEval);

		if (fEval)
			break;
	}


	printf("Elaborate mean :%f\n",elaborateMean/(float)i);
	printf("Initial guess mean :%f\n",initGuessMean/(float)i);
	printf("Neg clause mean :%f\n",negClauseMean/(float)i);



	return fEval;

}


void CFormula::setGreedyFactor(float factor)
{
	this->greedyFactor=factor;
}

void CFormula::setGWFactor(float factor)
{
	this->gwFactor=factor;
}

std::vector<string> CFormula::tokenize(char*input,char*split)
{
	char * token;
	std::vector<string> token_vec;

	token = strtok (input,split);
	while (token != NULL)
	{
		token_vec.push_back(string(token));
		token = strtok (NULL, " ");
	}
	return token_vec;
}

Clause CFormula::parseClause(char*input,char*split)
{
	char * token;
	Clause cl;

	token = strtok (input,split);
	while (token != NULL)
	{
		Literal lit=stoi(token);
		if (lit!=0)
		{
			int index=abs(lit);
			// -- update the stats

			this->varMap[index].push_back(clCounter);

			if (lit>0)
			{
				this->fStats.nPosLits++;
				this->fPortions[index]+=1;
			}
			else this->fStats.nNegLits++;
			fStats.nLits++;
			this->nLit[index]++;
			// --

			cl.push_back(lit);
		}
		token = strtok (NULL, " ");
	}

	this->clCounter++;

	return cl;
}




FillMethod CFormula::getRandomMethod()
{
	return (FillMethod)rndMethod(rndEngine);
}


HeuristicMethod CFormula::getRandomHeuristic()
{
	return (HeuristicMethod)rndHeuristic(rndEngine);
}



bool CFormula::takeBestChoice()
{

	double random=rndReal(rndEngine);	// -- Get a double random number between 0 and 1

	// -- Check if GSAT should take a best action
	if (random<=this->greedyFactor)
		return true;
	return false;
}

bool CFormula::takeGW()
{

	double random=rndReal(rndEngine);	// -- Get a double random number between 0 and 1

	// -- Check if GSAT should take a best action
	if (random<=this->gwFactor)
		return true;
	return false;
}


void CFormula::evalMove(int&min,int&max)
{
	min=std::numeric_limits<int>::max();
	max=0;

	for (int i=1;i<this->nVars;i++)
	{
		int clNum=0;
		int temp=0;
		toggle(this->fVars[i]);

		this->evalFormulaQ(i,clNum);	//this->evalFormula(clNum);
		toggle(this->fVars[i]);
		this->evalFormulaQ(i,temp);

		this->evalArray[i]=clNum;
		if (clNum>max) max=clNum;
		if (clNum<min) min=clNum;

	}
}

std::vector<int> CFormula::evalUnsatClause(int clIndex,int&min,int&max)
{
	min=std::numeric_limits<int>::max();
	max=0;

	Clause cl=this->cFormula[clIndex];
	std::vector<int> evalArray;

	for (int i=0;i<cl.size();i++)
	{
		Literal lit=cl[i];
		int index=abs(lit);

		int clNum=0;
		int temp=0;

		// -- evaluate the positive clauses
		toggle(this->fVars[index]);
		this->evalFormulaQ(index,clNum);	//this->evalFormula(clNum);
		toggle(this->fVars[index]);
		this->evalFormulaQ(index,temp);
		// --

		evalArray.push_back(clNum);
		if (clNum>max) max=clNum;
		if (clNum<min) min=clNum;

	}

	return evalArray;
}

int CFormula::getBestChoice(bool retFirst)
{
	int min,max;
	this->evalMove(min,max);
	int bestCount=0;
	std::vector<int> bestArray;

	for (int i=1;i<this->nVars;i++)
	{
		if (this->evalArray[i]==max)
		{
			if (retFirst) return i;	// -- return immediately the first choice

			bestArray.push_back(i);
			bestCount++;
		}
	}

	if (bestCount==1)
		return bestArray[0];

	// -- randomly select the best choice
	int rndIndex=(int)(rndReal(rndEngine)*bestCount);
	return bestArray[rndIndex];

}


int CFormula::walkSatChoice(bool retFirst)
{

	// -- Randomly get one unsatisfied clause
	int clIndex=this->getUnsatClause(retFirst);

	int min,max;

	// -- List the best choices
	std::vector<int> evalArray=evalUnsatClause( clIndex,min,max);

	// -- select randomly from the best choices
	int bestCount=0;
	std::vector<int> bestArray;

	for (int i=0;i<evalArray.size();i++)
	{
		if (evalArray[i]==max)
		{
			if (retFirst) return i;	// -- return immediately the first choice

			bestArray.push_back(i);
			bestCount++;
		}
	}

	if (bestCount==1)
		return abs(this->cFormula[clIndex].at(bestArray[0]));

	// -- randomly select the best choice
	int rndIndex=(int)(rndReal(rndEngine)*bestCount);

	return abs(this->cFormula[clIndex].at(bestArray[rndIndex]));
}

int CFormula::getUnsatClause(bool retFirst)
{

	int clCount=0;
	std::vector<int> clArray;

	for (int i=0;i<this->nClauses;i++)
	{
		if (!this->evalClause[i])
		{
			if (retFirst) return i;	// -- return immediately the first choice

			clArray.push_back(i);
			clCount++;
		}
	}

	if (clCount==1)
		return clArray[0];

	// -- randomly select the best choice
	int rndIndex=(int)(rndReal(rndEngine)*clCount);
	return clArray[rndIndex];

}

int CFormula::getRndBest(bool exponential)
{
	int min,max;
	float cum=0;
	this->evalMove(min,max);
	float normalizer;

	// -- calculate the discrete probability distribution --
	float  probs[this->nVars];
	float rnd =rndReal(rndEngine);

	for (int i=1;i<this->nVars;i++)
	{

		probs[i]=(float)(this->evalArray[i]-min+DEFAULT_BIAS);

		if (exponential)probs[i]=exp(probs[i]);
		normalizer+=probs[i];
	}

	// -- sample from the distribution --
	for (int i=1;i<this->nVars;i++)
	{
		cum+=probs[i]/normalizer;

		if (cum>rnd)
			return i;

	}

	return 0;
}




