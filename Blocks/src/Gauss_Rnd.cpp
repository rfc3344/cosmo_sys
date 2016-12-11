#include "Gauss_Rnd.h"
#include "Uniform_Rnd.h"
#include "stdio.h"
#include "time.h"

#define GAUSS_GEN_RANK 10

static double sqrtOfN[] = {1.000000000000000, 1.414213562373095, 1.732050807568877,  
                           2.000000000000000, 2.236067977499790, 2.449489742783178,    
                           2.645751311064591, 2.828427124746190, 3.000000000000000,  
                           3.162277660168380, 3.316624790355400, 3.464101615137754,
                           3.605551275463989, 3.741657386773941, 3.872983346207417,
                           4.000000000000000, 4.123105625617661, 4.242640687119285};

double gauss(double mean, double sigma, long long * seed0, long long * seed1)
{
	long i;
	double x, y;

	double x1 = 0;
	
	for (int rank = 0; rank < GAUSS_GEN_RANK; rank++)
	{
		x = 0;
		for (i = 0; i < 12; i++)
		{
			x += uniform(0.0, 1.0, seed0, seed1);
		}
		x -= 6.0;
		x1 += x;
	}

	x1 = x1 / sqrtOfN[GAUSS_GEN_RANK - 1];
	y = mean + x1 * sigma;

	return y;
}


Gauss_Rnd::Gauss_Rnd(double mean, double sigma, long seed0, long seed1, int DataRate)
	: gauss_factory(mean, sigma, seed0, seed1)
{
	_dataRate = DataRate;
}

Gauss_Rnd::~Gauss_Rnd()
{
}

bool Gauss_Rnd::run()
{
	Connector * con = getConnector(conIdx);

	if (con == 0)
	{
		return true;
	}

	ConnectorData_T sndData;
	
	for (int idx = 0; idx < _dataRate; idx++)
	{
		double t = gauss_factory.getInstance();
		
		sndData.dbl_buf[0] = t;

		con->Send(sndData);
	}

	return true;
}


Gauss_Factory::Gauss_Factory(double mean, double sigma, long seed0, long seed1)
{
	_mean  = mean;
	_sigma = sigma;
	_seed0 = seed0;
	_seed1 = seed1;
}

Gauss_Factory::~Gauss_Factory()
{
}

double Gauss_Factory::getInstance()
{
	return gauss(_mean, _sigma, &_seed0, &_seed1);
}

double Gauss_Factory::getInstance(double mean, double sigma)
{
	return gauss(mean, sigma, &_seed0, &_seed1);
}

