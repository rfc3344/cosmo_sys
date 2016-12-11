#include "MsgBase.h"
#include "ErrorHandling.h"
#include "string.h"

int MsgBase::MsgNum = 0;
int MsgBase::MsgCode[MsgCodeNumMax_Total];

MsgSender *   MsgBase::sender[MsgCodeNumMax_Total];

MsgBase::MsgBase(void)
{
}

MsgBase::~MsgBase(void)
{
}

void MsgInitialize()
{
	MsgBase::MsgNum = 0;
	memset(MsgBase::MsgCode, 0, sizeof(MsgBase::MsgCode));
	memset(MsgBase::sender, 0, sizeof(MsgBase::sender));
}

MsgSender::MsgSender(void)
{
	RegisteredMsgCodeNum = 0;
	memset(RegisteredMsgCode, 0, sizeof(RegisteredMsgCode));
	
	memset(listenerTbl, 0, sizeof(listenerTbl));
}

MsgSender::~MsgSender(void)
{
}

int MsgSender::RegisterMsgSender(int MsgCode)
{
	if ((RegisteredMsgCodeNum < MsgCodeNumMax_1Sender) 
		&&
		(MsgBase::MsgNum < MsgCodeNumMax_Total))
	{
		RegisteredMsgCode[RegisteredMsgCodeNum] = MsgCode;
		RegisteredMsgCodeNum++;

		MsgBase::MsgCode[MsgBase::MsgNum] = MsgCode;
		MsgBase::sender[MsgBase::MsgNum]  = this;
		MsgBase::MsgNum++;

		return MsgCode;
	} 
	else
	{
		ErrorMsg(GeneralError, __FILE__, __LINE__, 
			     "No more Msg types can be registered (RegisteredMsgCodeNum = %d MsgBase::MsgNum = %d)", 
			     RegisteredMsgCodeNum,
			     MsgBase::MsgNum);
	}
	
	return -1;
}

void MsgSender::addListener(int MsgCode, MsgReceiver * receiver)
{
	for (int MsgTypeIdx = 0; MsgTypeIdx < RegisteredMsgCodeNum; MsgTypeIdx++)
	{
		if (RegisteredMsgCode[MsgTypeIdx] == MsgCode)
		{
			if (listenerTbl[MsgTypeIdx].receiverNum < MsgRciverMaxNum_1Sender)
			{
				listenerTbl[MsgTypeIdx].receiver[listenerTbl[MsgTypeIdx].receiverNum] = receiver;
				listenerTbl[MsgTypeIdx].receiverNum++;
			}
			else
			{
				ErrorMsg(GeneralError, __FILE__, __LINE__, 
						 "Exceeding the Maximum number of msg listener! ");
			}
			return;
		}
	}
	
	ErrorMsg(GeneralError, __FILE__, __LINE__, 
		     "I do not send msg (code:%d)", MsgCode);
	
}

void MsgSender::notify(int MsgCode, const MsgBody_T& MsgData)
{
	for (int MsgIdx = 0; MsgIdx < RegisteredMsgCodeNum; MsgIdx++)
	{
		if (RegisteredMsgCode[MsgIdx] == MsgCode)
		{
			for (int listenerIdx = 0 ; listenerIdx < listenerTbl[MsgIdx].receiverNum; listenerIdx++)
			{
				(listenerTbl[MsgIdx].receiver[listenerIdx])->MsgObserver(MsgCode, MsgData);
			}
			return;
		}
	}
}

MsgReceiver::MsgReceiver(void)
{
}

MsgReceiver::~MsgReceiver(void)
{
}

void MsgReceiver::RegisterMsgReceiver(int MsgCode)
{
	/* find out the sender of the msg identified by MsgCode */
	MsgSender * msgSender = 0;
	
	for (int MsgTypeIdx = 0; MsgTypeIdx < MsgBase::MsgNum; MsgTypeIdx++)
	{
		if (MsgCode == MsgBase::MsgCode[MsgTypeIdx])
		{
			msgSender = MsgBase::sender[MsgTypeIdx];
			break;
		}
	}

	/* insert receiver pointer to sender's listening list */

	if (msgSender == 0)
	{
		ErrorMsg(GeneralError, __FILE__, __LINE__, 
			     "The MsgCode (%d) is never registered ! ", 
			     MsgCode);
		return;
	}

	msgSender->addListener(MsgCode, this);
}

void MsgReceiver::MsgObserver(int MsgCode, const MsgBody_T& MsgData)
{
}



