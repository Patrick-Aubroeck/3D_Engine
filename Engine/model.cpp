// standard library includes
#include <fstream>

// own includes
#include "model.h"
#include "loggermanager.h"

///<summary> standard constructor </summary>
Model::Model() :
	m_vertexCount(0),
	m_model(NULL),
	m_indices(NULL),
	m_vertices(NULL)
{

}

///<summary> standard destructor </summary>
Model::~Model()
{

}

/// <summary>Initialize a model with a texture and initialize the buffers</summary>
/// <param name ="renderer">the renderer that will initialize the buffers </param>
/// <param name ="resourceManager"> the resourcemanager that will load in the model and the texture</param>
/// <returns>succes yes/no? </returns>
/// <seealso cref="ResourceManager.LoadModel()" />
/// <seealso cref="InitializeBuffers()" />
/// <seealso cref="ResourceManager.LoadTexture()" />
bool Model::Initialize(I_Renderer* renderer,ResourceManager* resourceManager)
{
	bool result;
	
	// we use our own .txt files, there is a obj file converter that makes obj files into txt files the engine can read
	m_model = resourceManager->LoadModel("../Engine/data/sphere.txt",m_indiceCount,m_vertexCount);

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(renderer, "../Engine/data/seafloor.dds");
	if(!result)
	{
		return false;
	}

	result = resourceManager->LoadTexture("../Engine/data/seafloor.dds");

	return true;
}

/// <summary>Initialize the buffers by setting up the vertices and indices and give this to the renderer</summary>
/// <param name ="renderer"> the renderer that will call the createModel function </param>
/// <param name ="texturefilepath"> the filepath of the texture, use this as identifier for the map in the renderer</param>
/// <returns>succes yes/no? </returns>
/// <seealso cref="I_Renderer.CreateModel()" />
bool Model::InitializeBuffers(I_Renderer* renderer, std::string texturefilepath)
{
	//Create the index array
	
	m_indices = new unsigned long[m_indiceCount];
	if(!m_indices)
	{
		return false;
	}
	// set the values of the indices
	for(int i=0; i<m_vertexCount; i++)
	{
		m_indices[i] = i;
	}

	//creates a buffer for this model
	renderer->CreateModel(m_model , m_vertexCount, m_indices, m_indiceCount, texturefilepath);
	
	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] m_vertices;
	m_vertices = NULL;

	delete [] m_indices;
	m_indices = NULL;

	return true;
}

/// <summary>get the position, texture, color and normals of the model</summary>
/// <returns> return the vertices as a CustomVertexType </returns>
struct CustomVertexType* Model::GetVertices()
{
	return m_vertices;
}

/// <summary>return the position, texture, normal of the vertice </summary>
/// <returns>returns the vertice as a CustomModelType </returns>
struct CustomModelType* Model::GetModel()
{
	return m_model;
}

/// <summary> get the amount of indices</summary>
/// <returns> return the pointer to the indices </returns>
unsigned long* Model::GetIndices()
{
	return m_indices;
}

/// <summary>get the amount of vertices</summary>
/// <returns>returns the amount of vertices as an integer </returns>
int Model::GetVertexCount()
{
	return m_vertexCount;
}

/// <summary>delete the vertices and indices if they still exist and set them to NULL</summary>
void Model::Shutdown()
{
	if(m_vertices)
	{
		delete[] m_vertices;
		m_vertices = NULL;
	}
	if(m_indices)
	{
		delete[] m_indices;
		m_indices = NULL;
	}
}

/// <summary>Set the vertex and index buffer ready for the graphic pipeline by calling RenderBuffers</summary>
/// <param name ="renderer"> </param>
/// <param name ="texturepathname"> the path of the texture used as identifier </param>
void Model::Render(I_Renderer* renderer, std::string texturepathname)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(renderer, texturepathname);
}

/// <summary>Set the buffers ready on the graphic pipeline to be rendered</summary>
/// <param name ="renderer">  the renderer that has to call the setupbuffer function</param>
/// <param name ="texturepathname"> the texturepathname used as an identifier</param>
/// <seealso cref="Renderer.SetupBuffer()" />
void Model::RenderBuffers(I_Renderer* renderer, std::string texturepathname)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(CustomModelType); 
	offset = 0;
    
	renderer->SetupBuffer(stride, offset, texturepathname);

}
