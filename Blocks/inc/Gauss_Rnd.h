#ifndef _GAUSS_RND_H_
#define _GAUSS_RND_H_

#include "BlockBase.h"

class Gauss_Factory
{
public:
	Gauss_Factory(double mean, double sigma, long seed0, long seed1);
	~Gauss_Factory();

	double getInstance();
	double getInstance(double mean, double sigma);
private:
	double _mean;
	double _sigma;
	
	long long _seed0;
	long long _seed1;	
};

class Gauss_Rnd : public DataGenerator
{
public:
	Gauss_Rnd(double mean, double sigma, long seed0, long seed1, int DataRate);
	~Gauss_Rnd();

	int conIdx;

	bool run();
	
private:
	Gauss_Factory gauss_factory;	
	int    _dataRate;
};

double gauss(double mean, double sigma, long long * seed0, long long * seed1);


#endif

