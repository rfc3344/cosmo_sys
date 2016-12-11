#include "ErrorHandling.h"
#include "BlockBase.h"
#include "SysSchduler.h"


BlockBase::BlockBase(void)
{
	_inputInterfaceNum = 0;
}

BlockBase::~BlockBase(void)
{
}

void BlockBase::UpdateRunablity()
{
	bool rTrue  = true;
	
	for (int cEndIdx = 0; cEndIdx < _inputInterfaceNum; cEndIdx++)
	{	
		ConnectorEnd * cEnd = cnntorEnd[cEndIdx];
		bool b = cEnd->getRunability();

		rTrue  = rTrue && b;
	}

	if (rTrue)
	{
		/* runnable */
		//MsgPrint("Runnable!");
		registerRunnable((Runable * )this);
	}
	else
	{
		/* not runable */ 
		//MsgPrint("Not Runnable!");
		deRegisterRunnable((Runable * )this);
	}
	
}

bool BlockBase::run()
{
	return true; /* keep runing */
}

void BlockBase::addDataConnectorEnd(ConnectorEnd * cEnd)
{
	if (_inputInterfaceNum < InputInterfaceMaxNum)
	{
		cnntorEnd[_inputInterfaceNum] = cEnd;
		_inputInterfaceNum++;
	}
}

DataGenerator::DataGenerator(void)
{
	for (int idx = 0; idx < OutputInterfaceMaxNum; idx++)
	{
		_ConArray[idx] = 0;
	}

	_ConnectorNum = 0;
}

DataGenerator::~DataGenerator(void)
{
}

bool DataGenerator::isStavationExist()
{
	if (_ConnectorNum <= 0)
	{
		WaringMsg(GeneralError, __FILE__, __LINE__, "The Generator has no next block to feed data to..");
	}
	
	for (int idx = 0; idx < _ConnectorNum; idx++)
	{
		Connector * con = _ConArray[idx];

		if (con->isStavationExist())
		{
			return true;
		}
	}
	return false;
}

int DataGenerator::getConnectorNum()
{
	return _ConnectorNum;
}

Connector * DataGenerator::getConnector(int ConIdx)
{
	if (ConIdx >= 0 && ConIdx < _ConnectorNum)
	{
		return _ConArray[ConIdx];
	}
	
	return 0;
}

int DataGenerator::addConnector(Connector * con)
{
	if (con == 0)
	{
		ErrorMsg(ParameterIncorrect, __FILE__, __LINE__, "add a Null connector ?? ");
		return -1;
	}

	if (_ConnectorNum >= OutputInterfaceMaxNum)
	{
		ErrorMsg(ParameterIncorrect, __FILE__, __LINE__, "the number of Connector equals to Max, no more connector is allowed! ");
		return -1;
	}

	_ConArray[_ConnectorNum] = con;

	int ConIdx = _ConnectorNum;
	
	_ConnectorNum++;

	return ConIdx;
}

void DataGenerator::UpdateRunablity()
{
	ErrorMsg(GeneralError, __FILE__, __LINE__, "This Function shoule never be called! ");
}

void DataGenerator::addDataConnectorEnd(ConnectorEnd * cEnd)
{
	ErrorMsg(GeneralError, __FILE__, __LINE__, "This Function shoule never be called! ");
}

