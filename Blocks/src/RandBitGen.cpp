#include "RandBitGen.h"

RandBitGen::RandBitGen(int DataRate, int seed1, int seed2)
	: guass_factor(0, 1, seed1, seed2)
{
	OutputIdx = -1;

	_DataRate = DataRate;
}

RandBitGen::~RandBitGen(void)
{
}

bool RandBitGen::run()
{
	Connector * con = getConnector(OutputIdx);

	if (!con)
	{
		return true;
	}

	ConnectorData_T sndData;

	for (int dataIdx = 0; dataIdx < _DataRate; dataIdx++)
	{
		for (int idx = 0; idx < sizeof(sndData) / sizeof(sndData.i_buf[0]); idx++)
		{
			int BitData = (guass_factor.getInstance() > 0) ? 1 : 0;
			sndData.i_buf[idx] = BitData;
		}
		
		con->Send(sndData);
	}

	return true;
}
