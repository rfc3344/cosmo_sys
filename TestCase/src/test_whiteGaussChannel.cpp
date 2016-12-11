#include "SysSchduler.h"
#include "RandBitGen.h"

void testWhiteGaussChannel()
{
	SystemInit();

    double snr = -2;

    int Count = 10000000;

    RandBitGen RndBitSource(10, 1243423, 0);
    registerDataGenerator(&RndBitSource);

}
