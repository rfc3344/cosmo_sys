#include "math.h"
#include "QPSKModulator.h"

#define OneOverSQRT2 0.707106781186547

/*
*    b(i) b(i+1)
*
*     10  |  00 
*     ---------
*     11  |  01
*/

QPSKModulator::QPSKModulator()
	: Input((BlockBase *)this)
{
	Input.setDataRate(_DataRate);
	addDataConnectorEnd(&Input);

	pOutput = 0;
}

QPSKModulator::~QPSKModulator()
{
}

bool QPSKModulator::run()
{
	ConnectorData_T dataReceived;
	ConnectorData_T sndData;

	int Bit[2];

	for (int idx = 0; idx < _DataRate; idx++)
	{
		dataReceived = Input.get();

		/* 00 -> (1/sqrt(2)) * [ 1 + 1i]  */
 		/* 01 -> (1/sqrt(2)) * [ 1 - 1i]  */
		/* 10 -> (1/sqrt(2)) * [-1 + 1i]  */
		/* 11 -> (1/sqrt(2)) * [-1 - 1i]  */
		
		Bit[idx] = -2 * dataReceived.i_buf[0] + 1;   
	}

	if (!pOutput)
	{
		return true;
	}

	sndData.dbl_buf[0] = OneOverSQRT2 * (double)(Bit[0]); /* real part */
	sndData.dbl_buf[1] = OneOverSQRT2 * (double)(Bit[1]); /* imag part */

	pOutput->Send(sndData);

	return true;
}
