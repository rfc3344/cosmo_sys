#include "Runable.h"


static int ObjId = 1;

void RunableInit()
{
	ObjId = 1;
}

int getUniqueId()
{
	return ObjId++;
}

Runable::Runable(void)
{
	_RunableOjbId = getUniqueId();
}

int * Runable::getObjId()
{
	return &_RunableOjbId;
}

Runable::~Runable(void)
{
}

