#include "skydome.h"

/// <summary>standard constructor</summary>
Skydome::Skydome() : 
	m_model(NULL),
	m_gradientColor(),
	m_indiceCount(0),
	m_vertexCount(0)
{

}

/// <summary>standard destructor</summary>
Skydome::~Skydome()
{

}

/// <summary>initialize the skydome by loading in the model and setting the buffers up with the right information</summary>
/// <param name ="renderer"> the renderer that has to initialize the buffers </param>
/// <param name ="skyboxmodelName"> the path of the skyboxmodel </param>
/// <returns> succes yes/no?</returns>
/// <seealso cref="LoadSkydomeModel()" />
/// <seealso cref="InitializeBuffers()" />
bool Skydome::Initialize(I_Renderer* renderer, std::string skyboxmodelName)
{
	bool result;

	
	// Load in the sky dome model.
	result = LoadSkydomeModel(skyboxmodelName);
	if(!result)
	{
		return false;
	}

	// Load the sky dome into a vertex and index buffer for rendering.
	result = InitializeBuffers(renderer, skyboxmodelName);
	if(!result)
	{
		return false;
	}

	return true;
}

/// <summary>get the gradientcolor of the skydome</summary>
/// <returns> the gradientcolor as a customGradientBufferType holding the apex and center color </returns>
CustomGradientBufferType Skydome::GetGradientColor()
{
	return m_gradientColor;
}

/// <summary>Initialize data for the buffers and let the renderer create the buffers with the data</summary>
/// <param name ="renderer"> the renderer that needs to call the createskydome function</param>
/// <param name ="skyboxmodelname"> the path of the skydomemodel</param>
/// <returns>succes yes/no? </returns>
/// <seealso cref="Renderer.CreateSkydome()" />
bool Skydome::InitializeBuffers(I_Renderer* renderer,  std::string skyboxmodelname)
{
	bool result;
	CustomVector3* vertices;
	unsigned long* indices;
	// Create the vertex array.
	vertices = new CustomVector3[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indiceCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for(int i = 0; i<m_vertexCount; i++)
	{
		vertices[i].t_x = m_model[i].position.t_x;
		vertices[i].t_y = m_model[i].position.t_y;
		vertices[i].t_z =  m_model[i].position.t_z;
		indices[i] = i;
	}

	result = renderer->CreateSkydome(vertices,m_vertexCount,indices, m_indiceCount, skyboxmodelname);
	if(!result)
	{
		return false;
	}
	
	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = NULL;

	delete [] indices;
	indices = NULL;

	return true;
}

/// <summary>shutdown the skydome and its member variables</summary>
void Skydome::Shutdown()
{
	if(m_model)
	{
		delete[] m_model;
		m_model = NULL;
	}
}

/// <summary> set the gradientcolor using rgba values</summary>
/// <param name ="gradientColor">the gradientcolor that should be used </param>
void Skydome::SetGradientColor(CustomGradientBufferType gradientColor)
{
	m_gradientColor = gradientColor;
}

/// <summary> setup the buffers ready for rendering</summary>
/// <param name ="renderer">the rendere that should setup the buffers for rendering </param>
/// <param name ="skyboxmodelname">the path of the skyboxmodel </param>
void Skydome::Render(I_Renderer* renderer, std::string skyboxmodelname)
{
	RenderBuffers(renderer, skyboxmodelname);
}

/// <summary>read in a txt file model and set the texture position and normals of each vertex</summary>
/// <param name ="filename"> the path of the modelfile </param>
/// <returns>succes yes/no? </returns>
bool Skydome::LoadSkydomeModel(std::string filename)
{
	std::ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);
	
	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indiceCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new CustomModelType[m_vertexCount];
	if(!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].position.t_x >> m_model[i].position.t_y >> m_model[i].position.t_z;
		fin >> m_model[i].texture.t_x >> m_model[i].texture.t_y;
		fin >> m_model[i].normal.t_x >> m_model[i].normal.t_y >> m_model[i].normal.t_z;
	}

	// Close the model file.
	fin.close();

	return true;
}

/// <summary>get the indexCount</summary>
/// <returns>return the indiceCount as an integer </returns>
int Skydome::GetIndexCount()
{
	return m_indiceCount;
}

/// <summary>setup the buffers ready for the graphic pipeline to draw</summary>
/// <param name ="renderer"> the renderer that needs to setup the buffers ready for the graphic pipeline </param>
/// <param name ="skyboxmodelname"> the path of the modelfile </param>
/// <seealso cref="Renderer.SetupBuffer()" />
void Skydome::RenderBuffers(I_Renderer* renderer, std::string skyboxmodelname)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(CustomVector3); 
	offset = 0;
	renderer->SetupBuffer(stride, offset, skyboxmodelname);
}