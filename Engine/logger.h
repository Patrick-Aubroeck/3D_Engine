#ifndef _LOGGER_H_
#define _LOGGER_H_

//standard includes
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

enum MessageType
{
	FatalError, // Crash the application
	Error, // Crash the application
	Warning, // warning 
	Loading, // Loading 
	Saving, // Saving
	Succes // succesfull
};

class Logger
{
private:
	std::string m_logfile; // the logfile directory
	std::string m_loggerName; // the class who is logging (Mouse/Kernel/etc)
public:
	Logger(std::string loggername); // constructor. ~~WARNING~~ use the loggermanager to create a logger ~~WARNING~~
	virtual ~Logger(); //destructor
	void WriteMessage(std::string, MessageType messageType); // writes a message to the logfile with a messagetype.
	void DeleteLogFile(); //delete the directory of the logger that is calling this function
private:
	std::string ConvertEnumToString(MessageType messageType); // converts the given messagetype to a string representation
};

#endif
