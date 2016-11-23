#ifndef _SCENE_H_
#define _SCENE_H_

// standard includes
#include <string>
#include <vector>

// own includes 
#include "i_renderer.h"
#include "model.h"
#include "camera.h"
#include "shader.h"
#include "timer.h"
#include "mouse.h"
#include "light.h"
#include "terrain.h"
#include "skydome.h"

class Scene
{
private:	

	I_Renderer* m_renderer; // holds the renderer
	I_Window* m_window; // vector?? zodat we meerdere windows supporten ( for now 1 window )
	std::map <std::string , Entity*> m_models; // eventueel nog een lijstje van maken 
	Shader* m_shader; // holds the shader 
	Camera* m_camera; // holds the active camera
	Timer* m_time; // holds the actual time 
	Light* m_light; // holds all the lighting variables
	Terrain* m_terrain; // holds the terrain variables
	Skydome* m_skydome; //holds the skydome variables

public:

	Scene(); //standard constructor
	virtual ~Scene(); // standard destructor
	
	void BeginScene(float red, float green, float blue, float alpha); //starts up the backbuffer , and give it the background color given as param.
	void EndScene(); // starts the flipping of the back to the frontbuffer
	void Render(ResourceManager* resourceManager); // calls begin and endscene
	void Shutdown(); // shutdown
	void CreateScene(ResourceManager* ResourceManager); // creates a standard scene 
	void UpdateCamera(Keyboard* keyboard, Mouse* mouse); // updates the camera with the with the input given from mouse or keyboard
	bool Initialize(); // initialize the member variables of the scene
	
	void CreateModel(std::string modelName);

	void SetRenderer(I_Renderer* renderer); // set the scene renderer as the one given as param
	void SetOutputWindow(I_Window* window); // sets the scene window as the one given as param
	
	I_Renderer* GetRenderer(); //returns the active renderer object
	I_Window* GetWindow(); // returns the active window object
	Entity* GetEntity(std::string entityName); // return the entity with the given name as param
	Shader* GetShader(); // returns the active shader object
	Camera* GetCamera(); // returns the active camera object
	Timer* GetTimer(); //returns the active timer object
	Light* GetLight(); //returns the active light object
	Terrain* GetTerrain(); //returns the active terrain object
	Skydome* GetSkydome(); //returns the active skydome object
};

#endif