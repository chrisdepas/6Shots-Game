#ifndef __DEBUGUTIL_H__
#define __DEBUGUTIL_H__

#include "CDebugLogger.h"
#include <stdlib.h> 

/*	
	Christopher De Pasquale
	Created:		20th of April, 2018
	Last Edit:		24th of April, 2018

Usage: 
		DbgAssert(pPtr != NULL);
		DbgAssertMsg(pPtr != NULL, "The pointer is null!!!");
*/

#ifdef NDEBUG
	#define DbgAssertMsg(ignore, _ignore) ((void)0)
	#define DbgAssert(ignore) ((void)0)
#else 

	#define DbgAssertMsg(expr, msg) \
		if (!expr == false) { \
			CDebugLogger::LogFatalFile("ASSERT_LOG.txt", "ASSERT FAILED - %s\n\tSource: %s AT LINE %i\n\tEXPRESSION: %s != 1\n", msg, __FILE__, __LINE__, #expr); \
			exit(EXIT_FAILURE); \
		}

	#define DbgAssert(expr) \
		if (expr == false) { \
			CDebugLogger::LogFatalFile("ASSERT_LOG.txt", "ASSERT FAILED!!\n\tSource: %s AT LINE %i\n\tEXPRESSION: %s != 1\n", __FILE__, __LINE__, #expr); \
			exit(EXIT_FAILURE); \
		}

#endif



#endif