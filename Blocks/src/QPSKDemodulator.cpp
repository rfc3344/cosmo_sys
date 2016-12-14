#include "BlockBase.h"
#include "Connector.h"
#include "QPSKDemodulator.h"

/*
*    b(i) b(i+1)
*
*     10  |  00 
*     ---------
*     11  |  01
*/

QPSKDemodulator::QPSKDemodulator(int OutputType)
	: Input((BlockBase *)this)
{
	Input.setDataRate(1);
	addDataConnectorEnd(&Input);
	
	_outputType = OutputType;
}

QPSKDemodulator::~QPSKDemodulator()
{
}

bool QPSKDemodulator::run()
{
	ConnectorData_T getData = Input.get();
	ConnectorData_T sndData;

	if (pOutput == 0)
	{
		return true;
	}
	
	if (_outputType == OUTPUTHardBit)
	{
		sndData.i_buf[0] = (getData.dbl_buf[0]) > 0 ? 0 : 1;
		pOutput->Send(sndData);
		
		sndData.i_buf[0] = (getData.dbl_buf[1]) > 0 ? 0 : 1;
		pOutput->Send(sndData);
	}
	else
	{
		//@TODO: 
	}
	
	return true;
}


