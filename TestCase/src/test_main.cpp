#include "stdio.h"
#include "avltree.h"

int compareInt(int * key1, int * key2)
{
	if (key1[0] > key2[0])
	{
		return 1;
	}
	else if (key1[0] == key2[0])
	{
		return 0;
	}

	return -1;
}

typedef struct
{
	int idx;
	int data;
} db_t;

unsigned int listElem(void * key, void * data, void * arg)
{
	printf("\nkey = %3d data = %3d", ((int *)(key))[0], ((int *)(data))[0]);
}

int test_main()
{
	int i;

	db_t dbArray[10];

	dbArray[0].idx = 4;
	dbArray[1].idx = 14;
	dbArray[2].idx = 34;
	dbArray[3].idx = 244;
	dbArray[4].idx = 4;
	dbArray[5].idx = 24;
	dbArray[6].idx = 634564;
	dbArray[7].idx = 454;
	dbArray[8].idx = 584;
	dbArray[9].idx = 42;

	for (int tmp1 = 0; tmp1 < sizeof(dbArray) / sizeof(db_t); tmp1++)
	{
		dbArray[tmp1].data = dbArray[tmp1].idx;
	}

	avltree_t * avltree;

	avltree = avl_new((avlcompfn_t *)compareInt);



	for (int tmp1 = 0; tmp1 < sizeof(dbArray) / sizeof(db_t); tmp1++)
	{
		avl_insert(avltree, (void *)&(dbArray[tmp1].idx), (void *)&(dbArray[tmp1].data));
	}


	avl_iterate(avltree, listElem, NULL);

	printf("\nfoo returns %d\n", 100);

	return 0;
}
