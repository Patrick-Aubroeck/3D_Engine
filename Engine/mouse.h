#ifndef _MOUSE_H_
#define _MOUSE_H_

//own includes
#include "i_inputdevice.h"
#include "customstruct.h"

class Mouse : virtual public I_InputDevice
{
private:	
	int m_mouseX; // the x position of the mouse
	int m_mouseY; // the y position of the mouse
	LPDIRECTINPUTDEVICE8 m_mouseDevice; // pointer to the mousedevice
	LPDIRECTINPUT8 m_dInput; // pointer to directinput
	DIMOUSESTATE m_mouseState; // keeps track of the mousestate
	CustomVector2 m_lastMouseState; // holds the position of the mouse it was last recorded at
	I_Window* m_window; // the active window

public:
	Mouse(); // standard constructor
	virtual ~Mouse(); //standard destructor
	bool Initialize(); // initialize the mouse
	void Shutdown(); // shutdown the device
	bool GetDeviceState(); // gets the device state 
	void ProcessInput(); // process the input sets the m_mouseX and y to the updated position
	bool CreateDirectInputDevice(); // create a directinput 
	LPDIRECTINPUT8 GetDirectInputDevice(); // gets the directinput
	void SetCooperativeLevel(); // set the cooperativelevel to the active window
	void SetWindow(I_Window* window); // set the active window
	void GetMouseLocation(int& mouseX, int& mouseY); // gets the mouse x and y location
	long GetXPos(); // get the mouse x position
	long GetYPos(); // get the mouse y position


};
#endif