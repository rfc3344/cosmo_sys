#include "CosmoCommon.h"
#include "ErrorHandling.h"
#include <iostream>
#include "math.h"
#include <string.h>
#include <stdio.h>
#include "stdarg.h"
#include <stdlib.h>

using namespace std;

#define ERRORLOCATIONBUFLEN 300

void SysCleanUp();

static char ErrorLocationBuf[ERRORLOCATIONBUFLEN];

static char * ErrorLocation(const char * fileName, const int lineNum)
{
	int idx;
	int len1 = strlen(fileName);
	int len2 = (int)(log10((double)lineNum) + 5);

	memset(ErrorLocationBuf, 0, sizeof(ErrorLocationBuf));

	for (idx = 0; ((idx < ERRORLOCATIONBUFLEN) && (idx < len1)); idx++)
	{
		ErrorLocationBuf[idx] = fileName[idx];
	}

	snprintf(&ErrorLocationBuf[MIN(len1, ERRORLOCATIONBUFLEN - len2)],
			 len2,
 		     ": %d ",
		     lineNum);

	ErrorLocationBuf[ERRORLOCATIONBUFLEN - 1] = 0;

	return ErrorLocationBuf;
}

void ErrorMsg(ErrorCode_T ErrorCode, const char * fileName, const int lineNum, char * fmt, ...)
{
	cout << "ErrorCode (" << ErrorCode << "): " << endl << "  " << ErrorLocation(fileName, lineNum) << endl;
	
	memset(ErrorLocationBuf, 0, sizeof(ErrorLocationBuf));

	va_list args;
	va_start(args, fmt);
	
	vsnprintf(ErrorLocationBuf, sizeof(ErrorLocationBuf), fmt, args);

	va_end(args);
	
	cout << "  " << ErrorLocationBuf << endl;

	cout << "Error! Simulation can not continue, it is going to stop!!" << endl;

    //_getch();
	
	SysCleanUp();
	
	exit(1);
}

void WaringMsg(ErrorCode_T ErrorCode, const char * fileName, const int lineNum, char * fmt, ...)
{
	cout << "WaringCode (" << ErrorCode << "): " << endl << "  " << ErrorLocation(fileName, lineNum) << endl;
	
	memset(ErrorLocationBuf, 0, sizeof(ErrorLocationBuf));

	va_list args;
	va_start(args, fmt);
	
	vsnprintf(ErrorLocationBuf, sizeof(ErrorLocationBuf), fmt, args);

	va_end(args);
	
	cout << "  " << ErrorLocationBuf << endl;
}

void MsgPrint(char * fmt, ...)
{
	memset(ErrorLocationBuf, 0, sizeof(ErrorLocationBuf));

	va_list args;
	va_start(args, fmt);
	
	vsnprintf(ErrorLocationBuf, sizeof(ErrorLocationBuf), fmt, args);

	va_end(args);
	
	cout << ErrorLocationBuf << endl;	
}
