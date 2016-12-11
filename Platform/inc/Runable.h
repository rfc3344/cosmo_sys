#ifndef _RUNABLE_H_
#define _RUNABLE_H_

class Runable
{
public:
	Runable(void);
	~Runable(void);

	virtual bool run() = 0;
	virtual void UpdateRunablity() = 0;

	int * getObjId();
	
private:
	int _RunableOjbId;  /* Obj ID which is used to avltree operation */
};

#endif
