#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

//standard includes
#include <map>
#include <string>

//own includes
#include "scene.h"
#include "i_renderer.h"
#include "windowmanager.h"
#include "resourcemanager.h"
 
class SceneManager
{
private:
	std::map<std::string, Scene*> m_scenes; //vector with scenes , so it can manage all the scenes 
	I_Renderer* m_renderer; // holds the renderer
	WindowManager* m_windowManager; //  holds the windowmanager
	ResourceManager* m_resourceManager; // holds the resourcemanager
	Scene* m_currentScene; // holds the active scene

public:
	SceneManager(); // constructor
	virtual ~SceneManager(); //destructor
	void RenderScenes(); // render all the scenes that the scenemanager has in his m_scenes list
	bool Initialize(WindowManager* windowManager, I_Renderer* renderer, ResourceManager* resourceManager ); //initialize the scenemanager, and sets the windowmanager rendere and resourcemanager of the scenemanager
	void Shutdown(); // Calls the shutdown function of each scene it has in the m_scene list
	void CreateScene(std::string sceneName); //create a scene and add it to the list
	void SetupScene(I_Window* window,std::string sceneName); // creates a default scene setup
	Scene* GetScene( std::string sceneName); // get the scene with the given scene name
	Scene* GetCurrentScene(); // gets the activescene

};
#endif