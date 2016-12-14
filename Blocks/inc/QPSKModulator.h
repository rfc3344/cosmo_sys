#ifndef _QPSKMODULATOR_H_
#define _QPSKMODULATOR_H_

#include "BlockBase.h"
#include "Connector.h"

#include "Complex.h"
#include "ErrorHandling.h"

class QPSKModulator : public BlockBase
{
public:
	QPSKModulator();
	~QPSKModulator();
	
	bool run();	
	
	ConnectorEnd Input;   
	Connector *  pOutput;
	
private:
	const static int _DataRate = 2;
};


#endif
