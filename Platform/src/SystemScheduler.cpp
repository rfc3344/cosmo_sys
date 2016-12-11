#include <cstddef>
#include <stdlib.h>
#include "ErrorHandling.h"
#include "MsgBase.h"
#include "avltree.h"
//#include "PlotBlock.h"

#include "SysSchduler.h"
#include "List.h"

void ConnectorInit();
void RunableInit();

DataGeneratorList dataGeneratorList;

void DataGeneratorList::addDataGenerator(DataGenerator * blk)
{
	if (_DataGeneratorNum >= MaxDataGeneratorNum)
	{
		ErrorMsg(OutOfBufBoundery, __FILE__, __LINE__, "Can not add more DataGenerator");
	}

	_BlkList[_DataGeneratorNum] = blk;

	_DataGeneratorNum++;
}

DataGeneratorList::DataGeneratorList()
{
	_DataGeneratorNum = 0;
}

DataGeneratorList::~DataGeneratorList()
{
	_DataGeneratorNum = 0;
}

void DataGeneratorList::selfDestroy()
{
	_DataGeneratorNum = 0;
}

void DataGeneratorList::selfInit()
{
	_DataGeneratorNum = 0;
}

void DataGeneratorList::generateData()
{
	for (int BlkIdx = 0; BlkIdx < _DataGeneratorNum; BlkIdx++)
	{
		while (_BlkList[BlkIdx]->isStavationExist())
		{
			_BlkList[BlkIdx]->run();
		}
	}
}

static int Compare(void * key1, void * key2)
{
	int int1 = *((int *)key1);
	int int2 = *((int *)key2);
	return int1 - int2;
}

avltree_t * Scheduler;
list_t      RunableList; 

typedef struct 
{
	list_node_t pNode;
	Runable *   blk;
} RunableQueueNode;

#define OFFSET(TYPE, MEMBER, pMem) ((TYPE *)((char *)pMem - ((char *)(&(((TYPE *)0)->MEMBER)))))

void SystemInit()
{
	MsgInitialize();

	ConnectorInit();

	RunableInit();

	//MatlabEngineInit();

	dataGeneratorList.selfInit();
	Scheduler = avl_new((avlcompfn_t *)Compare);

	list_init(&RunableList);
}

void registerDataGenerator(DataGenerator * DGTor)
{
	dataGeneratorList.addDataGenerator(DGTor);
}

void registerRunnable(Runable * blk)
{
	int * key;
	key = blk->getObjId();
	
	void * data = avl_insert(Scheduler, key, (void *)blk);
	
	if (data == NULL)
	{
		RunableQueueNode * pRQNode = (RunableQueueNode *)malloc(sizeof(RunableQueueNode));
		
		if (!pRQNode)
		{
			COSMOERROR;
		}
		
		list_init_node(&(pRQNode->pNode));
		pRQNode->blk = blk;

		list_add_head(&RunableList, &(pRQNode->pNode)); /* ��������ж��У��ȴ����ֵ��� */
	}
}

void deRegisterRunnable(Runable * blk)
{
	int * key;
	key = blk->getObjId();
	
	void * data = avl_search(Scheduler, key);

	if (data == 0)
	{
		return;
	}
	
	avl_remove(Scheduler, key);

	list_node_t * pNode = list_remove_first(&RunableList);
	
	RunableQueueNode * pRQNode = OFFSET(RunableQueueNode, pNode, pNode);
	
	free(pRQNode);	
}

static Runable * getFirstRunable()
{
	list_node_t * firstNode = list_get_first(&RunableList);

	if (firstNode == NULL)
	{
		return NULL;
	}

	RunableQueueNode * pRQNode = OFFSET(RunableQueueNode, pNode, firstNode);

	return pRQNode->blk;
}

void SysCleanUp()
{
	//MatlabEngineCleanUp();
	avl_destroy(Scheduler, 0, 0);
	dataGeneratorList.selfDestroy();

	/* clean up Runable List */
	while (list_is_empty(&RunableList) == LISTFALSE)
	{
		list_node_t * pNode = list_remove_first(&RunableList);
		
		RunableQueueNode * pRQNode = OFFSET(RunableQueueNode, pNode, pNode);
		free(pRQNode);
	}
}

void SysRun()
{
	bool keepRuning = true;
	
	while (keepRuning) 
	{
		dataGeneratorList.generateData();
		
		/*  �÷���ʹ��ģ����������ȼ�ȡ���ڳ�ʼ��ģ����Ⱥ�˳�򣬲��������������������������ж��е�ģ��ӵ��������ȼ���
		    ��������ʹ�����ݱ�������������ʡConnector���ڴ� 
		Runable * blk = (Runable *)avl_smallest(Scheduler);
		*/

		Runable * blk = (Runable *)getFirstRunable(); /* ��������������ж��е�ģ������ */

		while (blk && keepRuning)
		{
			keepRuning = blk->run();

			/* blk = (Runable *)avl_smallest(Scheduler); */

			blk = (Runable *)getFirstRunable();
		}
	}

	SysCleanUp();
}

