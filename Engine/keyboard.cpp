
#define KEYDOWN(name, key) (name[key] & 0x80)

#include "keyboard.h" 

/// <summary>standard constructor </summary>
Keyboard::Keyboard() :
	m_keyboardDevice(NULL),
	m_dInput(NULL),
	m_window(NULL)
{

}

/// <summary>standard destructor </summary>
Keyboard::~Keyboard()
{

}

/// <summary> will initialize a keyboard using Dinput</summary>
/// <returns> succes yes/no?</returns>
/// <seealso cref="CreateDirectInputDevice()" />
/// <seealso cref="DINPUT.CreateDevice()" />
/// <seealso cref="DINPUT.SetDataFormat()" />
bool Keyboard::Initialize()
{
	HRESULT result;

	if(!m_dInput)
	{
		CreateDirectInputDevice();
	}
	result = m_dInput->CreateDevice(GUID_SysKeyboard, &m_keyboardDevice, NULL);
	if(FAILED(result))
	{
		return false;
	}
	result = m_keyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(result))
	{
		return false;
	}
	return true;
}

/// <summary>Shutdown the keyboard and its member variables</summary>
void Keyboard::Shutdown()
{
	if(m_keyboardDevice)
	{
	m_keyboardDevice->Unacquire();
	m_keyboardDevice->Release();
	m_keyboardDevice = NULL;
	}
	if(m_dInput)
	{
		m_dInput->Release();
		m_dInput = NULL;
	}
}

/// <summary>create a directinputdevice using version 0x0800 </summary>
/// <returns>succes yes/no? </returns>
bool Keyboard::CreateDirectInputDevice()
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
LPDIRECTINPUT8 Keyboard::GetDirectInputDevice()
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
bool Keyboard::GetDeviceState()
{
	HRESULT result;
	ZeroMemory(m_keyState, sizeof(m_keyState));
	result = m_keyboardDevice->GetDeviceState(sizeof(m_keyState), m_keyState);
	if(FAILED(result))
	{
		if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			result = m_keyboardDevice->Acquire();
		}
		else
		{
			return false;
		}
	}
	return true;
}
/// <summary>check if the key given as param is pressed or not</summary>
/// <param name ="argKeyIsPressed"> the key that needs to be checked for </param>
/// <returns> keydown yes/no? </returns>
bool Keyboard::ProcessInput(char argKeyIsPressed) 
{ 
	if(!SUCCEEDED( m_keyboardDevice))
	{
		GetDeviceState();
	}
	
	// Check if keybuffer contains given key
	int pressed = KEYDOWN(m_keyState, argKeyIsPressed);

	if( pressed == 0 )
	{
		return false;
	}
	else
	{
		return true;
	}
}

/// <summary>set the active window of the keyboard</summary>
/// <param name ="window"> the window it needs to be set to </param>
/// <remark> if the set was succesfull set the cooperativelevel of the keyboard</remark>
void Keyboard::SetWindow(I_Window* window)
{
	m_window = window;
	if(m_window != NULL)
	{
	SetCooperativeLevel();
	}
}

/// <summary>sets the keyboard cooperativelevel to foreground/nonexclusive</summary>
void Keyboard::SetCooperativeLevel()
{
	m_keyboardDevice->SetCooperativeLevel(dynamic_cast<Window*>(m_window)->GetWindowHandleInstance(),DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}