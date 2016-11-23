#include "loggermanager.h"

///<summary>set the default value of loggermanager to NULL</summary>
LoggerManager* LoggerManager::m_loggerManager = NULL;

///<summary>standard constructor</summary>
LoggerManager::LoggerManager()
{

}

///<summary> standard destructor call the shutdown function before delete </summary>
LoggerManager::~LoggerManager()
{

}

/// <summary>get the loggermanager if there was no loggermanager as of yet, create a new one</summary>
/// <returns>returns the Loggermanager </returns>
/// <remark>Singleton</remark>
LoggerManager* LoggerManager::GetLoggerManagerInstance()
{
	if(!m_loggerManager)
	{
		m_loggerManager = new LoggerManager();
	}
	return m_loggerManager;
}

/// <summary>Create a logger with a name given as param</summary>
/// <param name ="loggerName"> the name of the logger that has to be created </param>
/// <remark>add the logger to the m_loggers map </remark>
void LoggerManager::CreateLogger(std::string loggerName)
{
		Logger* logger = new Logger(loggerName);
		m_loggers[loggerName] = logger;
}

/// <summary>get the logger with the given loggername, if no logger exists yet create a new one</summary>
/// <param name ="loggerName">the name of the logger that has to be retrieved </param>
/// <returns>the logger with the given logger name </returns>
/// < seealso cref="CreateLogger()" />
Logger* LoggerManager::GetLogger(std::string loggerName)
{
	if(!m_loggers[loggerName])
	{
		CreateLogger(loggerName);
	}
	return m_loggers[loggerName];
}

/// <summary>delete the logger with the given logger name</summary>
/// <param name ="loggerName"> the name of the logger that has to be deleted </param>
void LoggerManager::DeleteLogger(std::string loggerName)
{
	if(m_loggers[loggerName])
	{
		delete m_loggers[loggerName];
	}
	else
	{
		std::cout << loggerName << "no logger with that name found, could not delete" << std::endl;
	}
}

/// <summary>delete all the loggers that are in the m_loggers map</summary>
void LoggerManager::DeleteAllLoggers()
{
	for(std::map<std::string, Logger*>::iterator it = m_loggers.begin(); it != m_loggers.end(); ++it) {
		delete it->second;
	}
}