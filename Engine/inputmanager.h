#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

//standard includes
#include <map>
#include <string>

//own includes
#include "i_inputdevice.h"

class InputManager
{
private:
	std::map<std::string,I_InputDevice*> m_inputDevices; // vector with all the inputdevices
public:
	InputManager(); // constructor	
	virtual ~InputManager(); // destructor 
	I_InputDevice* GetInputDevice(std::string inputDeviceName); // gets the input device given as param, if the input device doesnt exist yet, create one
	bool InitializeAllDevices(); // init the devices, call the init for each device in the m_inputDevices list
	bool GetAllDeviceStates(); // set all the device states for each device in the m_inputDevices list
	bool CreateInputDevice(std::string inputDeviceName); // create an inputdevice with the given name(Keyboard/Mouse supported)
	void Shutdown(); // shutdown all its member variables and calls the shutdown function for each inputdevice in the m_inputdevices

};
#endif