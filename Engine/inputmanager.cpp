#include <windows.h>

// own includes
#include "inputmanager.h"
#include "keyboard.h"
#include "mouse.h"
#include "loggermanager.h"

/// <summary>standard constructor</summary>
InputManager::InputManager() :
	m_inputDevices()
{

}

/// <summary>standard destructor</summary>
InputManager::~InputManager()
{

}

/// <summary>Create a inputdevice corresponding to the input device . For now only possible to create a keyboard and or mouse</summary>
/// <param name ="inputDeviceName"> the name of the device {Keyboard or Mouse}</param>
/// <returns>succes yes/no </returns>
bool InputManager::CreateInputDevice(std::string inputDeviceName)
{
	I_InputDevice* inputDevice;

	if(!m_inputDevices[inputDeviceName])
	{
		if(inputDeviceName.compare("Mouse") == 0)
		{
			inputDevice = new Mouse();
		}
		else if(inputDeviceName.compare("Keyboard") == 0)
		{
			inputDevice = new Keyboard();
		}
		m_inputDevices[inputDeviceName] = inputDevice;
	}
	return true;
}

/// <summary>get the inputdevice with the given name as param, if no device was found create a new one and return this</summary>
/// <param name ="inputDeviceName"> the name of the device that needs tobe returned </param>
/// <returns>the device that was found </returns>
/// <seealso cref="CreateInputDevice()" />
I_InputDevice* InputManager::GetInputDevice(std::string inputDeviceName)
{
	if(!m_inputDevices[inputDeviceName])
	{
		CreateInputDevice(inputDeviceName);
	}
	return m_inputDevices[inputDeviceName];
}

/// <summary>shutdown the inputmanager and its member variables</summary>
/// <seealso cref="I_InputDevice.Shutdown()" />
void InputManager::Shutdown()
{
	for(std::map<std::string, I_InputDevice*>::iterator it = m_inputDevices.begin(); it!=m_inputDevices.end(); ++it)
	{
		it->second->Shutdown();
	}
}

/// <summary>Call the initialize function of all the input device that are in the inputdevicesmap</summary>
/// <returns> succes yes/no? </returns>
/// <seealso cref="I_InputDevice.Initialize()" />
bool InputManager::InitializeAllDevices()
{
	bool result;
	for(std::map<std::string, I_InputDevice*>::iterator it = m_inputDevices.begin(); it!=m_inputDevices.end(); ++it)
	{
		result = it->second->Initialize();
		if(!result)
		{
			return false;
		}
	}
	return true;
}

/// <summary>get the devicestate of all the input devices that are in the inputdevicesmap</summary>
/// <returns> succes yes/no?</returns>
/// <seealso cref="I_InputDevice.GetDeviceState()" />
bool InputManager::GetAllDeviceStates()
{
	bool result;
	for(std::map<std::string, I_InputDevice*>::iterator it = m_inputDevices.begin(); it!=m_inputDevices.end(); ++it)
	{
		result = it->second->GetDeviceState();
		if(!result)
		{
			return false;
		}
	}
	return true;
}



