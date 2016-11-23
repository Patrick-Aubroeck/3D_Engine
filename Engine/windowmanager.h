#ifndef _WINDOWMANAGER_H_
#define _WINDOWMANAGER_H_

//standard library includes
#include <string>
#include <map>

//own includes
#include "window.h"
#include "i_window.h"

class WindowManager
{
private:
	std::map<std::string, I_Window*> m_windows; // container to hold the windows with the given windowname as key
public:
	WindowManager(); // constructor.
	virtual ~WindowManager(); //destructor.
	void UpdateWindows(); //update all the windows in the m_windows map
	void CreateNewWindow(std::string windowName); // creates a new window with the given windowname if the windowname doesn't already exists.
	void ShutdownWindow(std::string windowName); // delete the window with the given windowname.
	void Shutdown(); //delete all the windows that the windowslist currently hold.
	I_Window* GetWindow(std::string windowName); // returns the window with the given windowname.
	I_Window* GetActiveWindow(); // returns the window that is currently active, if none is active returns the first in the list
	int GetSize(); // returns the size(amount of windows in the m_windows list)
};

#endif
