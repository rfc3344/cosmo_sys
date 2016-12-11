
#include "Star.h"
#include "string.h"

Star::Star(int InputNum, int OutputNum, StarFun_t * pFunc, void * pArg)
{
	_inputNum  = InputNum;
	_outputNum = OutputNum;

	_pFunc = pFunc;

	_pArg = pArg;

	for (int idx = 0; idx < _inputNum; idx++)
	{
		pInput[idx] = new ConnectorEnd((BlockBase *)this);
		pInput[idx]->setDataRate(1);
		addDataConnectorEnd(pInput[idx]);
	}

	memset(pOutput, 0, sizeof(pOutput));
}

Star::~Star()
{
	for (int idx = 0; idx < _inputNum; idx++)
	{
		delete pInput[idx];
	}
}

bool Star::run()
{
	bool rlt;
	for (int idx = 0; idx < _inputNum; idx++)
	{
		getData[idx] = pInput[idx]->get();
	}

	if (_pFunc)
	{
		rlt = _pFunc(getData, sndData, _pArg);
	}

	for (int idx = 0; idx < _outputNum; idx++)
	{
		pOutput[idx]->Send(sndData[idx]);
	}

	return rlt;
}


static bool Multiply2(ConnectorData_T * getData, 
               		  ConnectorData_T * sndData,
                      void * pArg)
{
	sndData->dbl_buf[0] = getData->dbl_buf[0] * 2;

	return true;
}

demoStar::demoStar()
	: Star(1, 1, Multiply2, NULL)
{
}

demoStar::~demoStar()
{
}

/*
void demoStar::Multiply2(ConnectorData_T * getData, 
                   		 ConnectorData_T * sndData,
                         void * pArg)
{
	sndData->dbl_buf[0] = getData->dbl_buf[0] * 2;
}
*/
