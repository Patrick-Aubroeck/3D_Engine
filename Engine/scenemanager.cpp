#include "scenemanager.h"

/// <summary>standard constructor </summary>
SceneManager::SceneManager() :
	m_renderer(NULL),
	m_windowManager(NULL),
	m_resourceManager(NULL),
	m_currentScene(NULL),
	m_scenes()
{

}

/// <summary>standard destructor </summary>
SceneManager::~SceneManager()
{

}

/// <summary>initialize the windowmanager renderer and the resourcemanager</summary>
/// <param name ="windowManager">the windowmanager that should be used </param>
/// <param name ="renderer">the renderer that should be used </param>
/// <param name ="resourceManager"> the resourcemanager that should be used </param>
/// <returns>succes yes/no </returns>
bool SceneManager::Initialize(WindowManager* windowManager, I_Renderer* renderer, ResourceManager* resourceManager)
{
	if(!m_windowManager)
	{
		m_windowManager = windowManager;
	}
	if(!m_renderer)
	{
		m_renderer = renderer;
	}
	if(!m_resourceManager)
	{
		m_resourceManager = resourceManager;
	}
	return true;
}

/// <summary>create a scene for a default set up</summary>
/// <param name ="window">the window where the scene should be rendered on </param>
/// <param name ="sceneName"> the name of the scene </param>
/// <seealso cref="Scene.SetOutputWindow()" />
/// <seealso cref="Scene.SetRenderer()" />
/// <seealso cref="Scene.Initialize()" />
/// <seealso cref="Scene.CreateScene()" />
void SceneManager::CreateScene(std::string sceneName)
{
	Scene* scene = new Scene();
	m_scenes[sceneName] = scene;
	m_currentScene = scene;
}

/// <summary>create a scene for a default set up</summary>
/// <param name ="window">the window where the scene should be rendered on </param>
/// <param name ="sceneName"> the name of the scene </param>
/// <seealso cref="Scene.SetOutputWindow()" />
/// <seealso cref="Scene.SetRenderer()" />
/// <seealso cref="Scene.Initialize()" />
/// <seealso cref="Scene.CreateScene()" />
void SceneManager::SetupScene(I_Window* window, std::string sceneName)
{
	Scene* scene = new Scene();
	
	scene->SetOutputWindow(m_windowManager->GetWindow(window->GetWindowName()));
	scene->SetRenderer(m_renderer);
	scene->Initialize();
	scene->CreateScene(m_resourceManager);
	m_scenes[sceneName] = scene;
	m_currentScene = scene;
}

/// <summary>shutdown the scenemanager and call the shutdown function for each member variable</summary>
/// <seealso cref="Scene.Shutdown()" />
void SceneManager::Shutdown()
{

	// for each scene call the shutdown function
	for(std::map<std::string,Scene*>::iterator it = m_scenes.begin(); it!= m_scenes.end(); ++it)
	{
		it->second->Shutdown();
		delete it->second;
		it->second = NULL;
	}
	//just set the pointer on zero as it has already been called in the for loop
	if(m_currentScene)
	{
		m_currentScene = NULL;
	}

}

/// <summary>render the all the scenes that are in the m_scenesmap</summary>
/// <seealso cref="Scene.Render()" />
void SceneManager::RenderScenes()
{
	//for each scene call the frame function
	for(std::map<std::string,Scene*>::iterator it = m_scenes.begin(); it!= m_scenes.end(); ++it)
	{
		m_currentScene = it->second;
		it->second->Render(m_resourceManager);
	}
}

/// <summary>return the scene with the given scenename</summary>
/// <param name ="sceneName"> the name of the scene </param>
/// <returns>the scene with the given name as param </returns>
Scene* SceneManager::GetScene(std::string sceneName)
{
	if(m_scenes[sceneName])
	{
		CreateScene(sceneName);
	}
	return m_scenes[sceneName];
}

/// <summary>get the currentscene</summary>
/// <returns>return the currentscene</returns>
Scene* SceneManager::GetCurrentScene()
{
	return m_currentScene;
}