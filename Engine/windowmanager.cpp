#include "windowmanager.h"
#include "loggermanager.h"

/// <summary> standard constructor</summary>
WindowManager::WindowManager() 
{

}
/// <summary> the destructor, call the shutdown function before deletion</summary>
WindowManager::~WindowManager()
{

}

/// <summary>gets the window from the m_windowsmap with the windowname given as param, if no window exist will create a window with that name</summary>
/// <param name ="windowName">the name of the window that needs to be returned </param>
/// <returns> returns the window with the given windowname </returns>
/// <seealso cref="CreateNewWindow()"/>
I_Window* WindowManager::GetWindow(std::string windowName)
{	
	if(!m_windows[windowName])
	{
		CreateNewWindow(windowName);
	}
	return m_windows[windowName];
}

/// <summary>get the window from the m_windows map where the isactive variable is set on true</summary>
/// <returns> returns the window where the boolean isactive is set on true</returns>
/// <remark> ~Warning~ will crash if this function is called with no windows available ~Warning~</remark>
I_Window* WindowManager::GetActiveWindow()
{
	for(std::map<std::string, I_Window*>::iterator it = m_windows.begin(); it!=m_windows.end(); ++it) 
	{
		if(it->second->GetIsActive() == true)
		{
			return it->second;
		}
	}
	return m_windows.begin()->second;
}
/// <summary>gets the size of the m_windows map</summary>
/// <returns> the amount of windows in hte m_windows map as an integer </returns>
int WindowManager::GetSize()
{
	return m_windows.size();
}

/// <summary>creates a window with the given windowname as param, if the window with the windowname already exist do nothing</summary>
///	<summary>after creating also initialize the window with pre defined settings</summary>
/// <param name ="windowName"> the name of the window that you want to create</param>
/// <seealso cref="Window.Initialize()"/>
void WindowManager::CreateNewWindow(std::string windowName)
{
	if(!m_windows[windowName])
	{
		Window* window = new Window(windowName);
		window->Initialize();
		m_windows[windowName] = window;
		LoggerManager::GetLoggerManagerInstance()->GetLogger("WindowManager")->WriteMessage("Succesfully created a window", MessageType::Succes);
	}
	else
	{
		LoggerManager::GetLoggerManagerInstance()->GetLogger("WindowManager")->WriteMessage("Window already exist", MessageType::Warning);
	}
}

/// <summary>update all the windows in the m_windowsmap</summary>
/// <remark>calls the updatewindow of the window </remark>
/// <seealso cref="Window.UpdateWindow()"/>
void WindowManager::UpdateWindows()
{
	for(std::map<std::string, I_Window*>::iterator it = m_windows.begin(); it!=m_windows.end(); ++it)
	{
		it->second->UpdateWindow();
	}
}

/// <summary>shutdown the window with the windowname given as param</summary>
/// <param name ="windowName">the windowname of the window that needs to be deleted</param>
/// <seealso cref="Window.Shutdown()"/>
void WindowManager::ShutdownWindow(std::string windowName)
{
	if(m_windows[windowName])
	{
		m_windows[windowName]->Shutdown();
		delete m_windows[windowName];
		m_windows[windowName] = NULL;
	}
	else
	{
		LoggerManager::GetLoggerManagerInstance()->GetLogger("WindowManager")->WriteMessage("No window exist with the given windowname{DELETE}", MessageType::Warning);
	}
}

/// <summary>shutdown every window that the windowmanager has in its m_windowsmap</summary>
/// <seealso cref="Window.Shutdown()"/>
void WindowManager::Shutdown()
{
	std::string text = "Succesfully deleted the window with the windowname : ";
	if(!m_windows.empty())
	{
		for(std::map<std::string, I_Window*>::iterator it = m_windows.begin(); it!=m_windows.end(); ++it) 
		{
			text = text.append(it->second->GetWindowName());
			it->second->Shutdown();
			LoggerManager::GetLoggerManagerInstance()->GetLogger("WindowManager")->WriteMessage(text , MessageType::Succes);
		}
	}
}