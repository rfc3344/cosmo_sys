#ifndef _QPSKDEMODULATOR_H_
#define _QPSKDEMODULATOR_H_

#include "BlockBase.h"
#include "Connector.h"

#define OUTPUTLLR     0
#define OUTPUTHardBit 1

class QPSKDemodulator : public BlockBase
{
public:
	QPSKDemodulator(int OutputType);
	~QPSKDemodulator();

	bool run();

	ConnectorEnd Input;
	Connector *  pOutput;
	
private:
	int _outputType;    /* OUTPUTLLR OUTPUTHardBit */ 
};


#endif