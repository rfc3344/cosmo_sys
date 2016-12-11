#ifndef _BLOCKBASE_H_
#define _BLOCKBASE_H_

#include "Runable.h"
#include "Connector.h"

#define InputInterfaceMaxNum  16
#define OutputInterfaceMaxNum 16

class ConnectorEnd;

class BlockBase : public Runable
{
public:
	BlockBase(void);
	~BlockBase(void);

	bool run();
	void UpdateRunablity();

	void addDataConnectorEnd(ConnectorEnd * cEnd);
	
private:
	int _inputInterfaceNum;

	ConnectorEnd * cnntorEnd[InputInterfaceMaxNum];
};

class Connector;

class DataGenerator : public BlockBase
{
public:

	DataGenerator(void);
	~DataGenerator(void);
	
	bool isStavationExist();

	int getConnectorNum();
	Connector * getConnector(int ConIdx);

	int addConnector(Connector * con);

private:
	void UpdateRunablity(); /* hide this function for DataGenerator */
	void addDataConnectorEnd(ConnectorEnd * cEnd);

	int _ConnectorNum;

	Connector * _ConArray[OutputInterfaceMaxNum];
};
	
#endif
