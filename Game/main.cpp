#include "kernel.h"
#include "inputmanager.h"
#include "scenemanager.h"
#include "windowmanager.h"
#include "i_renderer.h"
#include "keyboard.h"
#include "mouse.h"

int main()
{

////	//Noob Interface
	Kernel* m_kernel;
	//Create the kernel
	m_kernel = new Kernel();
	m_kernel->RunEngine();
	// Shutdown and release the system object.
	m_kernel->Shutdown();
	delete m_kernel;

//	return 0;

	//Pro Interface 
	//bool result;
	//bool done = false;
	//Kernel* kernel;
	//kernel = new Kernel();
	//
	//result = kernel->Initialize();
	//if(!result)
	//{
	//	exit(-1);
	//}
	//kernel->GetWindowManager()->CreateNewWindow("Window1");
	//I_Window* window =  kernel->GetWindowManager()->GetWindow("Window1");
	//kernel->GetRenderer()->CreateDevice();
	//kernel->GetRenderer()->Initialize(window);
	//kernel->GetSceneManager()->Initialize(kernel->GetWindowManager(),kernel->GetRenderer(),kernel->GetResourceManager());
	//kernel->GetResourceManager()->SetRenderer(kernel->GetRenderer());
	//kernel->GetInputManager()->CreateInputDevice("Keyboard");
	//kernel->GetInputManager()->CreateInputDevice("Mouse");
	//kernel->GetInputManager()->InitializeAllDevices();
	//kernel->GetSceneManager()->CreateScene("MyfirstScene");
	//kernel->GetSceneManager()->GetScene("MyfirstScene")->SetRenderer(kernel->GetRenderer());
	//kernel->GetSceneManager()->GetScene("MyfirstScene")->SetOutputWindow(window);
	//Keyboard* keyboard = dynamic_cast<Keyboard*>(kernel->GetInputManager()->GetInputDevice("Keyboard"));
	//// game loop
	//while(!done)
	//{
	//	kernel->GetInputManager()->GetAllDeviceStates();
	//	if(keyboard->ProcessInput(DIK_ESCAPE))
	//	{
	//		done = true;
	//	}
	//	kernel->GetSceneManager()->GetScene("MyfirstScene")->BeginScene(1.0f, 0.0f, 0.0f, 1.0f);
	//	kernel->GetSceneManager()->GetScene("MyfirstScene")->EndScene();
	//}
}