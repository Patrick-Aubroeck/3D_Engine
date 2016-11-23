#ifndef _LOGGERMANAGER_H_
#define _LOGGERMANAGER_H_

// standard library includes
#include <string>
#include <map>

// own includes
#include "logger.h"

class LoggerManager
{
private:
	std::map<std::string, Logger*> m_loggers; // container to hold the loggers with loggername as keyvalue
	static LoggerManager* m_loggerManager; // keep track of itself so the singleton pattern can be used
public:
	static LoggerManager* GetLoggerManagerInstance(); // will be used to get an instance of the loggermanager, this will ensure that there will always be only 1 loggermanager
	Logger* GetLogger(std::string loggerName); // get the logger with the given loggername , if no logger with that name found will call createLogger
private:
	LoggerManager(); // constructor
	virtual ~LoggerManager(); // destructor
	void CreateLogger(std::string loggerName); //create a logger and add it to the m_loggersmap
	void DeleteLogger(std::string loggerName); // delete the logger with the given loggername
	void DeleteAllLoggers(); // remove everything from the m_loggers map , and the info they hold

};
#endif