/*
 * ArrayFiller.cpp
 *
 *  Created on: Oct 1, 2016
 *      Author: Sina M.Baharlou
 *      The class for filling 1-D boolean array
 */


#include "ArrayFiller.hpp"

ArrayFiller::ArrayFiller(bool* bArray,unsigned int arrSize):
arrSize(arrSize),
stepSize(DEFAULT_STEP_SIZE),
currStep(DEFAULT_CURR_STEP),
tRate(0),
pArray(NULL)
{

	// -- Initialize boolean array if it has not been initialized

	if (bArray==NULL)
		this->bArray=new bool[arrSize];
	else
		this->bArray=bArray;

	// -- Initializing sampling distribution --

	this->rndReal=std::uniform_real_distribution<float>(0.0,1.0);

	// -- Init random engine --

	struct timespec ts;
	unsigned theTick = 0U;
	clock_gettime( CLOCK_REALTIME, &ts );
	theTick  = ts.tv_nsec / 1000000;
	theTick += ts.tv_sec * 1000;
	this->rndEngine.seed(theTick);
}


bool* ArrayFiller::getArray()
{
	return this->bArray;
}

void ArrayFiller::setPortionArray(float* pArray)
{
	this->pArray=pArray;
}

void ArrayFiller::setPortionRate(float rate)
{
	this->tRate=rate;
}


bool* ArrayFiller::fillArray(FillMethod method)
{


	switch (method)
	{

	case ZEROS_FILL:	// -- fills with zeros (false)
		memset(this->bArray,0,this->arrSize*sizeof(bool));
		break;

	case ONES_FILL: // -- fills with ones (true)
		memset(this->bArray,1,this->arrSize*sizeof(bool));
		break;

	case UNIFORM_FILL: // -- uniformly fills with zeros and ones
		for (int i=0;i<this->arrSize;i++)
		{
			float rnd=this->rndReal(this->rndEngine);
			this->bArray[i]=(rnd<UNIFORM_RATIO);
		}
		break;

	case STEP_FILL:	// -- fills with step method
		for (int i=0;i<this->arrSize;i++)
		{
			float rnd=this->rndReal(this->rndEngine);
			this->bArray[i]=(rnd < this->currStep);
		}

		this->currStep+=this->stepSize; // -- increase the step size ( probability of having true value)
		if (this->currStep>1) this->currStep=0;
		break;

	case PORTION_FILL: // -- fills proportionally
		for (int i=0;i<this->arrSize;i++)
		{
			float rnd=this->rndReal(this->rndEngine);
			this->bArray[i]=(rnd < this->tRate);
		}
		break;

	case PORTION_EL_FILL: // -- fills each element proportionally

		if (this->pArray==NULL)
			break;


		for (int i=0;i<this->arrSize;i++)
		{
			float rnd=this->rndReal(this->rndEngine);
			this->bArray[i]=(rnd < this->pArray[i]);
		}
		break;

	}

	return this->bArray;

}



