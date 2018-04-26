#ifndef __CDEBUGLOGGER_H__
#define __CDEBUGLOGGER_H__

/*
	Christopher De Pasquale
	Created:	November 2016
	Last Edit:	August 2018

		
VERBOSITY LEVELS

	FATAL - Something very bad & unexpected happened, program must terminate

	ERROR - Something failed or has gone wrong, but we (might) be able to continue
 
	WARNING - Something weird happened, but we can definately continue. Might lose functionality
	
	INFO - A significant debug message (e.g. loaded file, wrote settings)

	DEBUG - Something normal happened. Should be helpful when trying to debug


USAGE
	CDebugLogger::Instance()->SetLogFile("LogFile_xyz.log")->
*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string>

class CDebugLogger {
public:
	enum ELoggingLevel {
	LOGLEVEL_DEBUG		= 1 << 0,
	LOGLEVEL_INFO		= 1 << 1,
	LOGLEVEL_WARNING	= 1 << 2,
	LOGLEVEL_ERROR		= 1 << 3,
	LOGLEVEL_FATAL		= 1 << 4,
	LOGLEVEL_DISABLED	= 1 << 5,
};

private:
	int m_iVerbosityFlags;

	/* Output file path */
	std::string m_sLogFile;

	/* Output info */
	bool m_bStdout;	// Output to stdout
	bool m_bFile;   // output to file
	bool m_bLoggingEnabled; 
	bool m_bPrintLogLevel;	// Prepend loglevel as string 

	ELoggingLevel m_eLastLogLevel;

	/* Singleton */
	CDebugLogger();

public:

	void Log(char* format, ...);
	
	/* Set current logging level */
	CDebugLogger* LogLevel(ELoggingLevel level = LOGLEVEL_DEBUG);

	/* Enable/Disable verbosity levels */
	CDebugLogger* SetVerbosityRange(ELoggingLevel Start, ELoggingLevel end = LOGLEVEL_DEBUG);
	CDebugLogger* SetVerbosityFlag(ELoggingLevel level, bool bEnabled = true);

	/* Output settings */
	CDebugLogger* SetLogFile(char* szLogFile, bool bEnableFile = true);
	CDebugLogger* SetUseFile(bool bEnable);
	CDebugLogger* SetUseStdout(bool bEnable);
	CDebugLogger* SetLogEnabled(bool bEnable);

	std::string GetLogFile() { return m_sLogFile; }
	bool GetUseFile() { return m_bFile; }

	static void LogDebug(char* format, ...);
	static void LogInfo(char* format, ...);
	static void LogWarning(char* format, ...);
	static void LogError(char* format, ...);
	static void LogFatal(char* format, ...);

	/* Log to a specific file without changing current output settings */
	static void LogErrorFile(char* szFile, char* format, ...);
	static void LogFatalFile(char* szFile, char* format, ...);

	static CDebugLogger* Instance() {
		static CDebugLogger logger;
		return &logger;
	}
};
#endif