#ifndef _STAR_H_
#define _STAR_H_

#include "BlockBase.h"
#include "Connector.h"

typedef bool StarFun_t(ConnectorData_T * getData, 
                       ConnectorData_T * sndData,
                       void * pArg);

class Star : public BlockBase
{
public:
	Star(int InputNum, int OutputNum, StarFun_t * pFunc, void * pArg);
	~Star();
	
	ConnectorEnd * pInput[InputInterfaceMaxNum];
	Connector    * pOutput[InputInterfaceMaxNum];
	
	bool run();
private:
	ConnectorData_T getData[InputInterfaceMaxNum];
	ConnectorData_T sndData[InputInterfaceMaxNum];

	int _inputNum;
	int _outputNum;

	StarFun_t * _pFunc;

	void * _pArg;
};

class demoStar : public Star
{
public:
	demoStar();
	~demoStar();
private:
};

#endif
