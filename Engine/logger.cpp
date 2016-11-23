#include "Logger.h"

///<summary>standard constructor </summary>
Logger::Logger(std::string loggername) :
	m_logfile("../Logs/" + loggername + ".txt"),
	m_loggerName(loggername)
{
	
}

///<summary> standard destructor</summary>
Logger::~Logger()
{

}

/// <summary>delete the file directory of the logger that is calling this function</summary>
void Logger::DeleteLogFile()
{
	m_logfile.clear();
}

/// <summary>write a message in the logfile</summary>
/// <param name ="message"> the message that has to be logged into the logfile </param>
/// <param name ="messageType"> Enumtype gives extra information about the message </param>
/// <remark>will set the current time and date using localtime </remark>
/// <seealso cref="ConvertEnumToString()" />
void Logger::WriteMessage(std::string message, MessageType messageType)
{
	std::ofstream myFile(m_logfile.c_str(), std::ios::out | std::ios::app);
	std::string errorType = ConvertEnumToString(messageType);

	time_t currenttime = time(0);
	tm* localtimePtr = localtime(&currenttime);

	if(myFile.is_open())
	{
		myFile <<"Date:" << localtimePtr->tm_mon << "-" << localtimePtr->tm_mday << "-" << localtimePtr->tm_year+1900;
		myFile << ":" << localtimePtr->tm_hour << ":" << localtimePtr->tm_min << ":" << localtimePtr->tm_sec;
		myFile << "\t{" << errorType << "} " << "\t" << message << std::endl;
	}
	myFile.close();

}

/// <summary>Converts the given enummessagetype to a printable string</summary>
/// <param name ="messageType"> the messagetype that has to be converted to a string</param>
/// <returns>the converted enum to string value </returns>
std::string Logger::ConvertEnumToString(MessageType messageType)
{
	std::string returnstring;

	switch(messageType)
	{
	case FatalError:
		returnstring = "FATALERROR";
		break;
	case Error:
		returnstring = "ERROR";
		break;
	case Warning:
		returnstring = "WARNING";
		break;
	case Loading:
		returnstring = "LOADING";
		break;
	case Saving: 
		returnstring = "SAVING";
		break;
	case Succes:
		returnstring = "SUCCES";
		break;
	default: 
		returnstring = "DEFAULT";
		break;
	}

	return returnstring;
}
