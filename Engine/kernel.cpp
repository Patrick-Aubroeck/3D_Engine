#include "kernel.h"
#include "d3d11.h"
#include "keyboard.h"
#include "mouse.h"

///<summary>standard constructor </summary>
Kernel::Kernel() :
	m_sceneManager(NULL),
	m_renderer(NULL),
	m_inputManager(NULL),
	m_windowManager(NULL),
	m_resourceManager(NULL)
{

}

/// <summary>standard destructor call the shutdown function first </summary>
Kernel::~Kernel()
{

}

/// <summary>Initialize the window,scene,resource,input managers and the renderer</summary>
/// <returns> succes yes/no?</returns>
bool Kernel::Initialize()
{
	m_windowManager = new WindowManager();
	if(!m_windowManager)
	{
		LoggerManager::GetLoggerManagerInstance()->GetLogger("Kernel")->WriteMessage("Creation of the windowmanager failed", MessageType::FatalError);
		return false;
	}
	m_renderer = new D3D11();
	if(!m_renderer)
	{
		LoggerManager::GetLoggerManagerInstance()->GetLogger("Kernel")->WriteMessage("Creation of the graphicsdevice failed", MessageType::FatalError);
		return false;
	}
	m_sceneManager = new SceneManager();
	if(!m_sceneManager)
	{
		LoggerManager::GetLoggerManagerInstance()->GetLogger("Kernel")->WriteMessage("Creation of the scenemanager failed", MessageType::FatalError);
		return false;
	}
	m_inputManager = new InputManager();
	if(!m_inputManager)
	{
		LoggerManager::GetLoggerManagerInstance()->GetLogger("Kernel")->WriteMessage("Creation of the inputmanager failed", MessageType::FatalError);
		return false;
	}	
	m_resourceManager = new ResourceManager();
	if(!m_resourceManager)
	{
		LoggerManager::GetLoggerManagerInstance()->GetLogger("Kernel")->WriteMessage("Creation of the resourcemanager failed", MessageType::FatalError);
		return false;
	}
	return true;
}

/// <summary>get the resourceManager</summary>
/// <returns>returns the resourceManager </returns>
ResourceManager* Kernel::GetResourceManager()
{
	return m_resourceManager;
}


/// <summary>will create and initialize a standard engine build </summary>
/// <remark> the standard build supports 1 window with 1 scene, a keyboard and a mouse </remark>
void Kernel::CreateDevices()
{
	//create a keyboard and a mouse device, and initialize them
	m_inputManager->CreateInputDevice("Keyboard");
	m_inputManager->CreateInputDevice("Mouse");
	m_inputManager->InitializeAllDevices();
	
	//declare a window
	I_Window* windowOne;
	
	//create window 1
	m_windowManager->CreateNewWindow("Window1");
	windowOne = m_windowManager->GetWindow("Window1");

	// create a device and deviceContext ( only 1 needed).
	m_renderer->CreateDevice();
	// create a swapchain rendertargetview etc etc ( 1 needed per window);
	m_renderer->Initialize(windowOne);

	// init the scenemanager with its own managers
	m_sceneManager->Initialize(m_windowManager, m_renderer, m_resourceManager);
	// set the resourcemanager its renderer
	m_resourceManager->SetRenderer(m_renderer);
	// create a scene by the given window.
	m_sceneManager->SetupScene(windowOne,"Scene1");
}

/// <summary>shutdown all the managers and the renderer</summary>
/// <remark>calls the shutdown function of all the managers and the renderer</remark>
/// <seealso cref="SceneManager.Shutdown()" />
/// <seealso cref="I_Renderer.Shutdown()" />
/// <seealso cref="InputManager.Shutdown()" />
/// <seealso cref="WindowManager.Shutdown()" />
/// <seealso cref="ResourceManager.Shutdown()" />
void Kernel::Shutdown()
{
	
	if(m_resourceManager)
	{
		m_resourceManager->Shutdown();
		delete m_resourceManager;
		m_resourceManager = NULL;
	}
	if(m_inputManager)
	{
		m_inputManager->Shutdown();
		delete m_inputManager;
		m_inputManager = NULL;
	}
	if(m_renderer)
	{
		m_renderer->Shutdown();
		delete m_renderer;
		m_renderer = NULL;
	}
	if(m_sceneManager)
	{
		m_sceneManager->Shutdown();
		delete m_sceneManager;
		m_sceneManager = NULL;
	}
	if(m_windowManager)
	{
		m_windowManager->Shutdown();
		delete m_windowManager;
		m_windowManager = NULL;
	}
}

/// <summary>get the active scenemanager</summary>
/// <returns>returns the scenemanager</returns>
SceneManager* Kernel::GetSceneManager()
{
	return m_sceneManager;
}

/// <summary>get the active inputmanager</summary>
/// <returns>returns the inputmanager </returns>
InputManager* Kernel::GetInputManager()
{
	return m_inputManager;
}

/// <summary>get the active renderer</summary>
/// <returns>returns the active renderer </returns>
I_Renderer* Kernel::GetRenderer()
{
	return m_renderer;
}

/// <summary>get the active windowmanager</summary>
/// <returns>returns the active windowmanager </returns>
WindowManager* Kernel::GetWindowManager()
{
	return m_windowManager;
}

/// <summary>will call all the update functions that is needed for a standard engine build</summary>
/// <returns> succes yes/no </returns>
/// <seealso cref="InputManager.GetInputDevice()" />
/// <seealso cref="InputManager.GetAllDeviceStates()" />
/// <seealso cref="WindowManager.UpdateWindows()" />
/// <seealso cref="Scene.UpdateCamera()" />
/// <seealso cref="SceneManager.RenderScenes()" />
bool Kernel::Frame()
{
	Keyboard* keyboard = dynamic_cast<Keyboard*>(m_inputManager->GetInputDevice("Keyboard"));
	Mouse* mouse = dynamic_cast<Mouse*>(m_inputManager->GetInputDevice("Mouse"));
	//update the keyboard and mouse 
	m_inputManager->GetAllDeviceStates();
	// update all the windows
	m_windowManager->UpdateWindows();
	// update camera position if input is given
	m_sceneManager->GetCurrentScene()->UpdateCamera(keyboard, mouse);
	// update all the scenes.
	m_sceneManager->RenderScenes();
	
	return true;
}

/// <summary>will create a standard engine build with 1 window 1 scene and a mouse and a keyboard</summary>
/// <returns> succes yes/no </returns>
/// <seealso cref="Initialize()" />
/// <seealso cref="CreateDevices()" />
/// <seealso cref="InputManager.GetInputDevice()" />
/// <seealso cref="WindowManager.GetActiveWindow()" />
/// <seealso cref="Frame()" />
void Kernel::RunEngine()
{
	bool result;

	// Initialize the inputmanager, scenemanager, windowmanager, resourcemanager and renderer.
	result = Initialize();	
	if(!result)
	{
		exit(-1);		
	}
	CreateDevices();

	Keyboard* keyboard = dynamic_cast<Keyboard*>(m_inputManager->GetInputDevice("Keyboard"));
	Mouse* mouse = dynamic_cast<Mouse*>(m_inputManager->GetInputDevice("Mouse"));

	while(result)
	{
		// every cycle update the keyboard and mouse m_window so it will always focus the window thats on the foreground at that time.
		keyboard->SetWindow(m_windowManager->GetActiveWindow());
		mouse->SetWindow(m_windowManager->GetActiveWindow());

		result = Frame();

		if(keyboard->ProcessInput(DIK_ESCAPE))
		{
			result = false;
		}
		
	}
}






