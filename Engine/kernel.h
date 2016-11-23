#ifndef _KERNEL_H_
#define _KERNEL_H_

//own includes
#include "windowmanager.h"
#include "inputmanager.h"
#include "i_renderer.h"
#include "sceneManager.h"
#include "loggermanager.h"
#include "resourcemanager.h"

class Kernel
{
private:
	SceneManager* m_sceneManager; // holds the scenemanager
	WindowManager* m_windowManager;// holds the windowmanager
	InputManager* m_inputManager; // holds the inputmanager
	ResourceManager* m_resourceManager; //holds the resourcemanager
	I_Renderer* m_renderer; // holds the renderer
public:
	Kernel(); // constructor
	virtual ~Kernel(); // destructor
	bool Initialize(); // initialize all its managers.
	void Shutdown(); // calls its member classes shutdown method if they exist.
	void RunEngine(); // will generate a standard engine build 

	SceneManager* GetSceneManager(); //returns the scenemanager
	WindowManager* GetWindowManager(); // returns the windowmanager
	InputManager* GetInputManager(); // returns the inputmanager
	I_Renderer* GetRenderer(); // returns the renderer
	ResourceManager* GetResourceManager(); //return the resourcemanager
private:
	void CreateDevices(); //the call to start a standard screen setup
	bool Frame(); // calls the update functions 
};
#endif