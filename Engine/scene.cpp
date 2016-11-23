#include "scene.h"
#include "settings.h"

/// <summary> standard constructor </summary>
Scene::Scene() :
	m_shader(NULL),
	m_camera(NULL),
	m_time(NULL),
	m_light(NULL),
	m_renderer(NULL),
	m_window(NULL),
	m_terrain(NULL),
	m_skydome(NULL),
	m_models()
{
	
}

///<summar>standard destructor call the shutdown function first</summary>
Scene::~Scene()
{

}

/// <summary>sets the scene renderer</summary>
/// <param name ="renderer"> the rendere the scene will use for rendering </param>
void Scene::SetRenderer(I_Renderer* renderer)
{
	m_renderer = renderer;
}

/// <summary>sets the output window</summary>
/// <param name ="window"> the window where the scene should draw itself on</param>
void Scene::SetOutputWindow(I_Window* window)
{
	m_window = window;
}

/// <summary>start of the drawing phase</summary>
/// <param name ="red"> the red color for the begin scene</param>
/// <param name ="green"> the green color for the begin scene</param>
/// <param name ="blue"> the blue color for the begin scene</param>
/// <param name ="alpha"> the transpirant value </param>
/// <remark> the higher the float in the param the more the color will show (255) max, (0) min
/// will call the renderer cleartargetrenderview and cleardepthstencil functions</remark>
void Scene::BeginScene(float red, float green, float blue, float alpha)
{
	
	float color[4];
	// Setup the color to clear the buffer to.
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	//clear the backbuffer and clear the depthstencil. to start with a fresh scene everytime.
	m_renderer->ClearTargetRenderView(color, m_window->GetWindowName());
	m_renderer->ClearDepthStencil(m_window->GetWindowName());

}

/// <summary>the end of the drawing phase</summary>
/// <remark> call at the end of the drawing phase, will call the renderer lockrefreshrate function</remark>
void Scene::EndScene()
{
	// present the scene to the frontbuffer
	m_renderer->LockRefreshRate(m_window->GetWindowName());
}

/// <summary>shutdown all the scens member variable and there shutdown function</summary>
void Scene::Shutdown()
{
	if(m_renderer)
	{
		m_renderer = NULL;
	}
	if(m_window)
	{
		m_window = NULL;
	}
	if(m_light)
	{
		m_light->Shutdown();
		delete m_light;
		m_light = NULL;
	}
	if(m_shader)
	{
		m_shader->Shutdown();
		delete m_shader;
		m_shader = NULL;
	}
	for(std::map<std::string, Entity*>::iterator it = m_models.begin(); it!=m_models.end(); ++it) 
	{
		it->second->Shutdown();
		delete it->second;
		it->second = NULL;
	}		
	if(m_camera)
	{
		m_camera->Shutdown();
		delete m_camera;
		m_camera = NULL;
	}
	if(m_time)
	{
		m_time->Shutdown();
		delete m_time;
		m_time = NULL;
	}
	if(m_terrain)
	{
		m_terrain->Shutdown();
		delete m_terrain;
		m_terrain = NULL;
	}
	if(m_skydome)
	{
		m_skydome->Shutdown();
		delete m_skydome;
		m_skydome = NULL;
	}
}

/// <summary>the main drawing function, in this function all the render functions of the member variables will be called used for the basic engine build</summary>
/// <param name ="resourceManager"> the resourcemanager that has to get the models, textures, etc</param>
/// <seealso cref="Skydome.SetGradientColor()" />
/// <seealso cref="Camera.Render()" />
/// <seealso cref="Renderer.GetWorldMatrix()" />
/// <seealso cref="Camera.GetViewMatrix()" />
/// <seealso cref="Skydome.SetGradientColor()" />
/// <seealso cref="Renderer.GetProjectionMatrix()" />
/// <seealso cref="Camera.GetPosition()" />
/// <seealso cref="Camera.Render()" />
/// <seealso cref="Renderer.TranslateMatrix()" />
/// <seealso cref="Renderer.TurnOffCulling()" />
/// <seealso cref="Renderer.TurnOffZBuffer()" />
/// <seealso cref="Skydome.Render()" />
/// <seealso cref="Shader.RenderSkybox()" />
/// <seealso cref="Renderer.TurnOnZBuffer()" />
/// <seealso cref="Renderer.TurnOnCulling()" />
/// <seealso cref="Terrain.Render()" />
/// <seealso cref="Renderer.Render()" />
/// <seealso cref="Entity.Render()" />

void Scene::Render(ResourceManager* resourceManager)
{
	//static float rotation = 0.0f;
	//// Update the rotation variable each frame.
	//rotation += (float)D3DX_PI * 0.005f;
	//if(rotation > 360.0f)
	//{
	//	rotation -= 360.0f;
	//}	
	CustomMatrix projection;
	CustomMatrix world;
	CustomMatrix view;
	//CustomMatrix ortho;
	int modelCount;
	CustomVector3 position;
	CustomVector4 color;
	CustomVector3 cameraPosition;
	CustomGradientBufferType gradientBuffer;
	BeginScene(0.0f, 0.0f, 1.0f, 1.0f);	
	gradientBuffer.apexColor = CustomVector4(0.01f, 0.01f, 0.06f, 1.0f);
	gradientBuffer.centerColor =  CustomVector4(0.88f, 0.88f, 0.88f, 1.0f);
	m_skydome->SetGradientColor(gradientBuffer);
	//genereate the view matrix.
	m_camera->Render(m_renderer,"Camera");
	// get the world projection and view matrix
	m_renderer->GetWorldMatrix(world, m_window->GetWindowName());
	m_camera->GetViewMatrix(view);
	m_renderer->GetProjectionMatrix(projection, m_window->GetWindowName());
	
	//skybox
	cameraPosition = m_camera->GetPosition();
	m_renderer->TranslateMatrix(cameraPosition, m_window->GetWindowName(), world);
	m_renderer->TurnOffCulling();
	m_renderer->TurnOffZBuffer();

	gradientBuffer = m_skydome->GetGradientColor();

	m_skydome->Render(m_renderer,"../Engine/data/skydome.txt");
	m_shader->RenderSkybox(m_renderer,m_skydome->GetIndexCount(),view, world, projection, gradientBuffer, "Skybox");

	m_renderer->TurnOnZBuffer();
	m_renderer->TurnOnCulling();
	m_renderer->GetWorldMatrix(world, m_window->GetWindowName());
	//end skybox

	m_terrain->Render(m_renderer, "../Engine/data/dirt01.dds");
	m_shader->Render(m_renderer, m_terrain->GetIndexCount(), "../Engine/data/dirt01.dds", view, world, projection, m_light->GetLighting(),"Default");

	modelCount = resourceManager->GetModelCount();

	for( int i = 0; i < modelCount; ++i)
	{
		resourceManager->GetData(i, position, color);
		for(std::map<std::string, Entity*>::iterator it = m_models.begin(); it!=m_models.end(); ++it) 
		{
			m_renderer->TranslateMatrix(position, m_window->GetWindowName(), world);
			it->second->Render(m_renderer,"../Engine/data/seafloor.dds");
			m_shader->Render(m_renderer,it->second->GetIndiceCount(), "../Engine/data/seafloor.dds", view, world, projection, m_light->GetLighting(),"Default");
			m_renderer->GetWorldMatrix(world ,m_window->GetWindowName());
		}
	}
	
	EndScene();	
}

/// <summary>initialize a standard scene setup for the standard engine build</summary>
/// <param name ="resourceManager">  the resourcemanager that has to get the information</param>
/// <seealso cref="CreateModel()" />
/// <seealso cref="Shader.Initialize()" />
/// <seealso cref="Shader.SetHLSLPsFileName()" />
/// <seealso cref="Shader.SetHLSLVsFileName()" />
/// <seealso cref="Terrain.Initialize()" />
/// <seealso cref="Timer.Initialize()" />
/// <seealso cref="Entity.Intialize()" />
/// <seealso cref="ResourceManager.Initialize()" />
/// <seealso cref="Light.SetDiffuseColor()" />
/// <seealso cref="Light.SetDirection()" />
/// <seealso cref="Light.SetAmbientColor()" />
void Scene::CreateScene(ResourceManager* resourceManager)
{
	std::string modelname = "Sphere";
	CreateModel(modelname);
	// the count .. how many models we want to see in the world
	int count = 6;
	
	m_skydome->Initialize(m_renderer,"../Engine/data/skydome.txt");
	m_shader->SetHLSLPsFileName("../Engine/skydome.ps");
	m_shader->SetHLSLVsFileName("../Engine/skydome.vs");
	m_shader->InitializeSkybox(m_renderer, "Skybox");

	m_terrain->Initialize(m_renderer, resourceManager, "../Engine/data/heightmap01.bmp", "../Engine/data/dirt01.dds",  "../Engine/data/colorm01.bmp", 9);
	m_time->Initialize();

	for(std::map<std::string, Entity*>::iterator it = m_models.begin(); it!=m_models.end(); ++it) 
	{
		it->second->Initialize(m_renderer,resourceManager);
		resourceManager->Initialize(count);
		
	}
	m_shader->SetHLSLPsFileName("../Engine/light.ps");
	m_shader->SetHLSLVsFileName("../Engine/light.vs");
	m_shader->Initialize(m_renderer, "Default");

	m_light->SetDiffuseColor(CustomVector4(1.0f, 1.0f, 0.0f, 1.0f));
	m_light->SetDirection(CustomVector3(-3.5f, -1.0f, 1.0f));
	m_light->SetAmbientColor(CustomVector4(0.05f, 0.05f, 0.0f, 1.0f));

}


/// <summary>get the the active camera of the scene</summary>
/// <return> return the active camera of the scene</return>
Camera* Scene::GetCamera()
{
	return m_camera;
}

/// <summary>get the light of the scene</summary>
/// <return> return the light of the scene </return>
Light* Scene::GetLight()
{
	return m_light;
}

/// <summary>get the the active camera of the scene</summary>
/// <return> return the shader </return>
Shader* Scene::GetShader()
{
	return m_shader;
}

/// <summary>get the the active skydome of the scene</summary>
/// <return> return the skydome of the scene</return>
Skydome* Scene::GetSkydome()
{
	return m_skydome;
}

/// <summary>get the active terrain of the scene</summary>
/// <return> return the terrain of the scene</return>
Terrain* Scene::GetTerrain()
{
	return m_terrain;
}

/// <summary>get the active timer of the scene</summary>
/// <return> return the timer of the scene</return>
Timer* Scene::GetTimer()
{
	return m_time;
}

/// <summary>get the entity with the name given as param</summary>
/// <param name ="modelName">  the name of the model that has to be returned</param>
/// <return> return the entity with the modelname as param</return>
Entity* Scene::GetEntity(std::string modelName)
{
	Entity* entity = m_models[modelName];
	return entity;
}

/// <summary>get the window that will be used to display the scene</summary>
/// <return> return the window that the scene will be rendered on</return>
I_Window* Scene::GetWindow()
{
	return m_window;
}

/// <summary>get the renderer that will be used to display the scene</summary>
/// <return>return the renderer that is used for drawing</return>
I_Renderer* Scene::GetRenderer()
{
	return m_renderer;
}

/// <summary>update the camera position by checking if the movement keys or the mouse has been used</summary>
/// <param name ="keyboard"> the keyboard that is used to process the movement keys </param>
/// <param name ="mouse">  the mouse that is used to check for mousemovements</param>
/// <seealso cref="Time.Frame()" />
/// <seealso cref="Time.GetFrameTime()" />
/// <seealso cref="Camera.SetFrameTime()" />
/// <seealso cref="Keyboard.ProcessInput()" />
/// <seealso cref="Camera.TurnLeft()" />
/// <seealso cref="Camera.TurnRight()" />
/// <seealso cref="Camera.MoveForward()" />
/// <seealso cref="Camera.MoveBackward()" />
/// <seealso cref="Camera.MoveUpward()" />
/// <seealso cref="Camera.MoveDownward()" />
/// <seealso cref="Camera.LookUp()" />
/// <seealso cref="Camera.LookDown()" />
/// <seealso cref="Mouse.ProcessInput()" />
/// <seealso cref="Camera.SetYawPitchRoll()" />
/// <seealso cref="Camera.GetYawPitchRoll()" />
/// <seealso cref="Mouse.GetXPos()" />
/// <seealso cref="Mouse.GetYPos()" />
void Scene::UpdateCamera(Keyboard* keyboard, Mouse* mouse)
{
	bool keydown;
	m_time->Frame();

	// set the frame time, this so we get a smooth turning speed that increases the longer you hold the button and slows down if you let it go.
	m_camera->SetFrameTime(m_time->GetFrameTime());

	//check if any of the keys are pressed, if so keydown will be true
	keydown = keyboard->ProcessInput(DIK_Q);
	m_camera->TurnLeft(keydown);
	keydown = keyboard->ProcessInput(DIK_E);
	m_camera->TurnRight(keydown);
	keydown = keyboard->ProcessInput(DIK_W);
	m_camera->MoveForward(keydown);
	keydown = keyboard->ProcessInput(DIK_S);
	m_camera->MoveBackward(keydown);
	keydown = keyboard->ProcessInput(DIK_A);
	m_camera->MoveUpward(keydown);
	keydown = keyboard->ProcessInput(DIK_Z);
	m_camera->MoveDownward(keydown);
	keydown = keyboard->ProcessInput(DIK_F);
	m_camera->LookUp(keydown);
	keydown = keyboard->ProcessInput(DIK_G);
	m_camera->LookDown(keydown);
	mouse->ProcessInput();

	m_camera->SetYawPitchRoll((float)mouse->GetXPos(),(float)mouse->GetYPos(), m_camera->GetYawPitchRoll().t_z);
	
	
}

/// <summary>create a Entity with the name given as param and add this Entity to the m_modelsmap</summary>
/// <param name ="modelName">  the name of the entity that will be used to identify the Entity </param>
void Scene::CreateModel(std::string modelName)
{
	Entity* model = new Model();
	m_models[modelName] = model;
}


/// <summary>Initialize a scene by creating a camera, shader, timer, light, terrain and skydome</summary>
/// <return> return true if succeeded, return false if any of the objects failed on creation </return>
bool Scene::Initialize()
{
	//create the class variable
	m_camera = new Camera();
	if(!m_camera)
	{
		//LoggerManager
		return false;
	}
	m_shader = new Shader();
	if(!m_shader)
	{
		//LoggerManager
		return false;
	}
	m_time = new Timer();
	if(!m_time)
	{
		//LoggerManager
		return false;
	}
	m_light = new Light();
	if(!m_light)
	{
		//LoggerManager
		return false;
	}
	m_terrain = new Terrain();
	if(!m_terrain)
	{
		//LoggerManager
		return false;
	}
	m_skydome = new Skydome();
	if(!m_skydome)
	{
		//LoggerManager
		return false;
	}
	return true;
}
