#ifndef _ERRORHANDLING_H_
#define _ERRORHANDLING_H_

typedef enum {
	NoError,
	MemAllocateFail,
	OutOfBufBoundery,
	ParameterIncorrect,
	FeatureNotSupportedYet,
	GeneralError
} ErrorCode_T;

void ErrorMsg(ErrorCode_T ErrorCode, const char * fileName, const int lineNum, char * msg, ...);
void WaringMsg(ErrorCode_T ErrorCode, const char * fileName, const int lineNum, char * fmt, ...);

void MsgPrint(char * fmt, ...);

#define COSMOERROR ErrorMsg(GeneralError, __FILE__, __LINE__, "Error!")

#endif