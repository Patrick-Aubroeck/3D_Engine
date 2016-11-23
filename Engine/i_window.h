#ifndef _I_WINDOW_H_
#define _I_WINDOW_H_

class I_Window
{
public:
	virtual const int GetScreenHeight() = 0;
	virtual const int GetScreenWidth() = 0;
	virtual const bool GetFullScreenSetting() = 0;
	virtual const bool GetIsActive() = 0; 
	virtual void Shutdown() = 0;
	virtual const std::string GetWindowName() = 0;
	virtual void UpdateWindow() = 0;
};

#endif