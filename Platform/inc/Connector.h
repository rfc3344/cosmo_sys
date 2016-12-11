#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "Runable.h"
#include "Complex.h"
#include "BlockBase.h"

#define MaxRecvrNum 20

class ConnectorEnd;

typedef union 
{
	char      c_buf[16];
	int       i_buf[4];
	long long l_buf[2];
	double    dbl_buf[2];  /* in case of transmitting Complex value, 
	                          idx0 is recommaned for real part, idx1 is for imag part */
} ConnectorData_T;

class Connector
{
public:
	
	Connector(int bufSz);
	~Connector(void);

	void Send(const ConnectorData_T& sndData);     /* Runtime function */
	
	ConnectorData_T getDataByConnectorEnd(int RecvrIdx);
	
	void RegisterRecvr(ConnectorEnd * RecvrBlock);  /* Building time function */

	bool isStavationExist(); /* called by Datagenerator to check whether 
	                            it should continue generating data */

	static int id;								
private:
	int _senderPointer;
	
	int _recverNum;
	int _recverPointer[MaxRecvrNum];
	int _recverDataRate[MaxRecvrNum];
	ConnectorEnd * _Recver[MaxRecvrNum];

	ConnectorData_T * _buf;
	int _bufSz;

	int _connectorId;
};

class BlockBase;

class ConnectorEnd
{
public:
	ConnectorEnd(BlockBase * Block);
	~ConnectorEnd();
	

	void setConnector(Connector * C, int RecvrIdx);  /* Building time function */
	
	int  getDataRate();
	void setDataRate(int DataRate);
	
	void setRunability(bool b);
	bool getRunability();
	
	ConnectorData_T get();                           /* Runtime function */
private:
	Connector * _Connector;
	int         _RecvrIdx;  
	int         _DataRate;
	bool        _runability;
	BlockBase * _Block;
};

void ConnectorInit();


#endif	
