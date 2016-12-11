#include "Connector.h"
#include "ErrorHandling.h"
#include "string.h"

int Connector::id = 0;

void ConnectorInit()
{
	Connector::id = 0;
}

static int getConnectorId()
{
	return Connector::id++;
}

Connector::Connector(int bufSz)
{
	_buf   = new ConnectorData_T[bufSz];
	_bufSz = bufSz;

	if (_buf == 0)
	{
		ErrorMsg(MemAllocateFail, __FILE__, __LINE__, "Heap mem allocate fails (%d)", bufSz);
	}
	
	_senderPointer = 0;
	
	_recverNum     = 0;

	memset(_recverPointer, 0, sizeof(_recverPointer));

	_connectorId = getConnectorId();
}

Connector::~Connector(void)
{
	delete []_buf;
	_senderPointer = 0;
	_recverNum     = 0;
	memset(_recverPointer, 0, sizeof(_recverPointer));
}

void Connector::RegisterRecvr(ConnectorEnd * RecvrBlock)
{
	if ((_recverNum >= MaxRecvrNum) || (_recverNum < 0))
	{
		ErrorMsg(OutOfBufBoundery, __FILE__, __LINE__, 
			     "RegisterRecvr fails (%d)", _recverNum);
	}

	if (RecvrBlock == NULL)
	{
		ErrorMsg(ParameterIncorrect, __FILE__, __LINE__, 
			     "RegisterRecvr a NULL ??");
	}

	_Recver[_recverNum]         = RecvrBlock;
	_recverDataRate[_recverNum] = RecvrBlock->getDataRate();

	RecvrBlock->setConnector(this, _recverNum);
	
	_recverNum++;
}

ConnectorData_T Connector::getDataByConnectorEnd(int RecvrIdx)
{
	if (RecvrIdx > _recverNum)
	{
		ErrorMsg(OutOfBufBoundery, __FILE__, __LINE__, 
			     "getDataByConnectorEnd fails (%d)", RecvrIdx);
	}

	if (_recverPointer[RecvrIdx] == _senderPointer)
	{
		ErrorMsg(ParameterIncorrect, __FILE__, __LINE__, 
			     "_recverPointer[RecvrIdx] == _senderPointer (%d) Check if bug exist in send function. \n \
			     the dataReceiver should not be runable. \n \
			     ConnectorID = %d", 
			     _recverPointer[RecvrIdx],
			     _connectorId);
	}

	ConnectorData_T Data = _buf[_recverPointer[RecvrIdx]];
	
	_recverPointer[RecvrIdx]++;

	if (_recverPointer[RecvrIdx] == _bufSz)
	{
		_recverPointer[RecvrIdx] = 0;
	}

	if ((_senderPointer + (_senderPointer >= _recverPointer[RecvrIdx] ? 0 : _bufSz) - _recverPointer[RecvrIdx]) < _recverDataRate[RecvrIdx])
	{
		_Recver[RecvrIdx]->setRunability(false);
	}

	return Data;

}

void Connector::Send(const ConnectorData_T& sndData)
{
	_buf[_senderPointer] = sndData;

	_senderPointer++;

	if (_senderPointer == _bufSz)
	{
		_senderPointer = 0;
	}

	for (int RcvrIdx = 0; RcvrIdx < _recverNum; RcvrIdx++)
	{
		if (_senderPointer == _recverPointer[RcvrIdx])
		{
			ErrorMsg(OutOfBufBoundery, __FILE__, __LINE__, 
				     "The buffer size is too small, not enough for recvr#(%d), please enlarger the bufSz. ConnectorId = %d ", 
				     RcvrIdx, 
				     _connectorId);			
		}
		
		if ((_senderPointer + (_senderPointer >= _recverPointer[RcvrIdx] ? 0 : _bufSz) - _recverPointer[RcvrIdx]) >= _recverDataRate[RcvrIdx])
		{
			/* check if the recvr is runable */
			_Recver[RcvrIdx]->setRunability(true);
		}
	}
}

bool Connector::isStavationExist()
{
	for (int RcvrIdx = 0; RcvrIdx < _recverNum; RcvrIdx++)
	{
		if ((_senderPointer + (_senderPointer >= _recverPointer[RcvrIdx] ? 0 : _bufSz) - _recverPointer[RcvrIdx]) < _recverDataRate[RcvrIdx])
		{
			return true;
		}
	}
	return false;
}

ConnectorEnd::~ConnectorEnd()
{
}

ConnectorEnd::ConnectorEnd(BlockBase * Block)
{
	_Connector = 0; 
	
	_runability = false;
	_RecvrIdx   = -1;  
	_DataRate   = 0;
	_Block      = Block;
}

void ConnectorEnd::setConnector(Connector * c, int RecvrIdx)
{
	_Connector = c;
	_RecvrIdx  = RecvrIdx;
}

ConnectorData_T ConnectorEnd::get()
{
	return _Connector->getDataByConnectorEnd(_RecvrIdx);
}

void ConnectorEnd::setDataRate(int DataRate)
{
	if (DataRate <= 0)
	{
		ErrorMsg(ParameterIncorrect, __FILE__, __LINE__, 
			     "DataRate should be positive! (%d)", DataRate);
	}
	
	_DataRate = DataRate;
}

int ConnectorEnd::getDataRate()
{
	return _DataRate;
}

void ConnectorEnd::setRunability(bool b)
{
	if (_runability != b)
	{
		_runability = b;
		_Block->UpdateRunablity();
	}
}

bool ConnectorEnd::getRunability()
{
	return _runability;
}

