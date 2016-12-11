#ifndef _UNIFORM_RND_H_
#define _UNIFORM_RND_H_

#include "BlockBase.h"

class Uniform_Rnd : public DataGenerator
{
public:
	Uniform_Rnd(double min, double max, long seed0, long seed1, int dataRate);
	~Uniform_Rnd();

	int conIdx; /* only one output interface */

	bool run();
private:
	double _min;
	double _max;
	long long _seed0;
	long long _seed1;
	int  _dataRate;
};

double uniform(double a, double b, long long * seed0, long long * seed1);

#endif