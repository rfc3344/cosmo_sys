#ifndef _SYSTEMSCHEDULER_H_
#define _SYSTEMSCHEDULER_H_

#include "BlockBase.h"
#include "List.h"

class DataGeneratorList
{
public:
	DataGeneratorList();
	~DataGeneratorList();

	void addDataGenerator(DataGenerator * blk);

	void generateData();

	void selfInit();
	void selfDestroy();
private:
	int _DataGeneratorNum;
	const static int MaxDataGeneratorNum = 50;
	DataGenerator *  _BlkList[MaxDataGeneratorNum];
};

void SystemInit();
void SysRun();
void SysCleanUp();

void registerDataGenerator(DataGenerator * DGTor);

void registerRunnable(Runable * blk);
void deRegisterRunnable(Runable * blk);

#endif