#include "mouse.h"

///<summary>standard constructor </summary>
Mouse::Mouse() :
	m_mouseDevice(NULL),
	m_dInput(NULL),
	m_lastMouseState(CustomVector2(0.0f, 0.0f)),
	m_window(NULL),
	m_mouseX(0),
	m_mouseY(0)
{

}

///<summary>standard destructor </summary>
Mouse::~Mouse()
{

}

/// <summary>get the current mouse position using a x and y coordinate</summary>
/// <param name ="mouseX">the output x position of the mouse </param>
/// <param name ="mouseY">the output y position of the mouse </param>
void Mouse::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
}

/// <summary> get the current x position of the mouse</summary>
/// <returns> the x position of the mouse</returns>
long Mouse::GetXPos()
{
	return m_mouseX;
}

/// <summary> get the current y position of the mouse</summary>
/// <returns> the y position of the mouse</returns>
long Mouse::GetYPos()
{
	return m_mouseY;
}

/// <summary> will initialize a mouse using Dinput</summary>
/// <returns> succes yes/no?</returns>
/// <seealso cref="CreateDirectInputDevice()" />
/// <seealso cref="DINPUT.CreateDevice()" />
/// <seealso cref="DINPUT.SetDataFormat()" />
bool Mouse::Initialize()
{
	m_mouseX = 0;
	m_mouseY = 0;

	HRESULT result;
	if(!m_dInput)
	{
		CreateDirectInputDevice();
	}
	result = m_dInput->CreateDevice(GUID_SysMouse, &m_mouseDevice, NULL);
	if(FAILED(result))
	{
		return false;
	}
	result = m_mouseDevice->SetDataFormat(&c_dfDIMouse);
	if(FAILED(result))
	{
		return false;
	}
	return true;
}

/// <summary>will call all the member variables shutdown function , also unacquire the mousedevice and release it</summary>
/// <seealso cref="DINPUT.Unacquire()" />
/// <seealso cref="DINPUT.Release()" />
void Mouse::Shutdown()
{
	if(m_mouseDevice)
	{
		m_mouseDevice->Unacquire();
		m_mouseDevice->Release();
		m_mouseDevice = NULL;
	}
	if(m_dInput)
	{
		m_dInput->Release();
		m_dInput = NULL;
	}
}

/// <summary>create a directinputdevice using version 0x0800 </summary>
/// <returns>succes yes/no? </returns>
bool Mouse::CreateDirectInputDevice()
{
	HRESULT result = DirectInput8Create(GetModuleHandle(NULL),DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&m_dInput,NULL);
	if(FAILED(result))
	{
		LoggerManager::GetLoggerManagerInstance()->GetLogger("inputManager")->WriteMessage("Error creating DirectInput",MessageType::FatalError);
		return false;
	}
	return true;
}

/// <summary> returns the directinputdevice of the device that calls it</summary>
/// <returns> return the directinput device</returns>
/// <remark> if there is no directinput set at the time of calling this function, it will create one and return it</remark>
/// <seealso cref="CreateDirectInputDevice()" />
LPDIRECTINPUT8 Mouse::GetDirectInputDevice()
{
	if(!m_dInput)
	{
		CreateDirectInputDevice();
	}
	return m_dInput;
}

/// <summary> get the device state of the mouse</summary>
/// <remark> if the devicestate is lost try to reaquire it, if that fails aswell will return a false. if the devicestate is active will call processinput</remark>
/// <returns> succes yes/no</returns>
bool Mouse::GetDeviceState()
{
	HRESULT result;
	
	result = m_mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE) , &m_mouseState);
	if(FAILED(result))
	{
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			result = m_mouseDevice->Acquire();
		}
		else
		{
			return false;
		}
	}
	ProcessInput();
	return true;
}

/// <summary>will set the x and y position of the mouse everytime it is called using the mousestate position as values</summary>
void Mouse::ProcessInput()
{
	if(m_window != NULL)
	{
		if(m_lastMouseState.t_x != m_mouseState.lX || m_lastMouseState.t_y != m_mouseState.lY)
		{
			m_lastMouseState.t_x = (float)m_mouseState.lX;
			m_mouseX += m_mouseState.lX;
			m_lastMouseState.t_y = (float)m_mouseState.lY;
			m_mouseY += m_mouseState.lY;
		}
	}
}

/// <summary>set the active window of the mouse</summary>
/// <param name ="window"> the window it needs to be set to </param>
/// <remark> if the set was succesfull set the cooperativelevel of the mouse</remark>
void Mouse::SetWindow(I_Window* window) 
{
	m_window = window;
	if(m_window != NULL)
	{
		SetCooperativeLevel();
	}
}

/// <summary>sets the mouse cooperativelevel to foreground/nonexclusive</summary>
void Mouse::SetCooperativeLevel()
{
	m_mouseDevice->SetCooperativeLevel(dynamic_cast<Window*>(m_window)->GetWindowHandleInstance(),DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}