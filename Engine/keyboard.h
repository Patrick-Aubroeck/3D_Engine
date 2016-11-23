#ifndef _KEYBOARD_H_ 
#define _KEYBOARD_H_

//own includes 
#include "i_inputdevice.h"

class Keyboard : virtual public I_InputDevice
{
private:
	unsigned char m_keyState[256]; // key array [256] 
	LPDIRECTINPUTDEVICE8 m_keyboardDevice; // keeps track of the directinputdevice (Keyboard)
	LPDIRECTINPUT8 m_dInput; // keeps track of the directinput
	I_Window* m_window; // the active window 

public:
	Keyboard(); // standard constructor
	virtual ~Keyboard(); // standard destructor
	bool Initialize(); // initialize the device
	void Shutdown(); // shutdown the device
	bool CreateDirectInputDevice(); // creates a directinput8
	LPDIRECTINPUT8 GetDirectInputDevice(); //get the directinput8
	bool GetDeviceState(); // get the device state
	bool ProcessInput(char keyPress); // process the given keypress as input
	void SetCooperativeLevel(); // set the cooperativelevel of the device
	void SetWindow(I_Window* window); // set the active window

};

#endif