#ifndef _MSGBASE_H_
#define _MSGBASE_H_

#define MsgRciverMaxNum_1Sender 10
#define MsgMaxNum_1Receiver     10

/* The maximum number of types of message for 1 sender */
#define MsgCodeNumMax_1Sender 5                  
#define MsgCodeNumMax_Total   1000

typedef union
{
	char c_buf[16];
	int  i_buf[4];
	double dbl_buf[2];
} MsgBody_T;

class MsgSender;
class MsgReceiver;

class MsgBase
{
public:
	MsgBase(void);
	~MsgBase(void);

	static int MsgNum;
	static int        MsgCode[MsgCodeNumMax_Total];
	static MsgSender * sender[MsgCodeNumMax_Total];
private:
		
};

class MsgSender : public MsgBase
{
public:
	MsgSender(void);
	~MsgSender(void);

	int RegisteredMsgCodeNum;
	int RegisteredMsgCode[MsgCodeNumMax_1Sender];
	
	int  RegisterMsgSender(int MsgCode);                         /* Building phase function */
	void addListener(int MsgCode, MsgReceiver * receiver);   /* Building phase function */
	
	void notify(int MsgCode, const MsgBody_T& MsgData);  /* Runing phase function */
private:	
	typedef struct
	{
		int           receiverNum;
		MsgReceiver * receiver[MsgRciverMaxNum_1Sender];
	} ListenerTbl;
	
	ListenerTbl listenerTbl[MsgCodeNumMax_1Sender];
};

class MsgReceiver : public MsgBase
{
public:
	MsgReceiver(void);
	~MsgReceiver(void);
	void RegisterMsgReceiver(int MsgCode);                              /* Building phase function */
	virtual void MsgObserver(int MsgCode, const MsgBody_T& MsgData);    /* Runing phase function */
private:	
};

void MsgInitialize();


#endif 
