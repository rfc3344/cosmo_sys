#ifndef _RANDBITGEN_H_
#define _RANDBITGEN_H_


#include "BlockBase.h"
#include "Gauss_Rnd.h"

class RandBitGen :
	public DataGenerator
{
public:
	RandBitGen(int DataRate, int seed1, int seed2);
	~RandBitGen(void);

	int OutputIdx; /* only one output interface */	

	bool run();
private:
	Gauss_Factory guass_factor;
	int _DataRate;
	
};

#endif	
