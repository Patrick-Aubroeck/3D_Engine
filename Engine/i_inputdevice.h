#ifndef _I_INPUTDEVICE_H_
#define _I_INPUTDEVICE_H_

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#include "loggermanager.h"
#include "i_window.h"
#include "window.h"

class I_InputDevice
{
public:
	virtual bool Initialize() = 0; 
	virtual void Shutdown() = 0;
	virtual bool GetDeviceState() = 0;
	virtual void SetWindow(I_Window* window) = 0;
	virtual void SetCooperativeLevel() = 0;

protected:
};
#endif