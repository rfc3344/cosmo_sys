#include "Uniform_Rnd.h"
#include "ErrorHandling.h"

#include "stdlib.h"

Uniform_Rnd::Uniform_Rnd(double min, double max, long seed0, long seed1, int dataRate)
{
	if (min > max)
	{
		ErrorMsg(ParameterIncorrect, __FILE__, __LINE__, "min(%f) > max(%f) ? ", min, max);
	}

	conIdx = -1;
	
	_min = min;
	_max = max;
	
	_seed0 = seed0;
	_seed1 = seed1;
	
	_dataRate = dataRate;
}

Uniform_Rnd::~Uniform_Rnd()
{
}

bool Uniform_Rnd::run()
{
	Connector * con = getConnector(conIdx);

	if (con == 0)
	{
		return true;
	}

	ConnectorData_T sndData;
	
	for (int idx = 0; idx < _dataRate; idx++)
	{
		double t;

		t = uniform(_min, _max, &_seed0, &_seed1);
			
		sndData.dbl_buf[0] = t;
		
		con->Send(sndData);
	}
	
	return true;
}


/* seed0 has to be different for different pesudo random sequence generating */

double uniform(double a, double b, long long * seed0, long long * seed1)
{
	static unsigned long long llMod = ((unsigned long long)1 << 42) - 1;

	static unsigned long long factor[] = {4194269, 4184317, 4194281, 4184345, 
		                                  4184289, 4194273, 4184301, 4194293, 
		                                  4184297, 4194285, 4184313};

	int factorIdx = (((* seed1) & 0xF) % 11); /* 0~10 */

	* seed0 = factor[factorIdx] * (* seed0) + 1;
	
	* seed0 = ((* seed0) & llMod);

	(* seed1) = (* seed1) + 16;

	if ((* seed1) >= ((long long)1 << 46))
	{
		(* seed1) = ((((* seed1) & 0xF) + 1) % 11);
	}
	
	double t = 1.0 * (double)(* seed0) / llMod;
	
	return a + (b - a) * t; 
}


