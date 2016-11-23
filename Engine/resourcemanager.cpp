#include "resourcemanager.h"

#include <stdlib.h>
#include <time.h>

///<summary> standard constructor</summary>
ResourceManager::ResourceManager() :
	m_renderer(NULL),
	m_modelList(NULL),
	m_model(NULL),
	m_modelCount(0),
	m_textures()
{

}

///<summary> standard destructor</summary>
ResourceManager::~ResourceManager()
{

}

/// <summary>will shutdown the resourcemanager and call the shutdown function of its member variables</summary>
void ResourceManager::Shutdown()
{
	if(m_modelList)
	{
		delete[] m_modelList;
		m_modelList = NULL;
	}
	for(std::map<std::string, Texture*>::iterator it = m_textures.begin(); it!= m_textures.end(); ++it) 
	{
		it->second->Shutdown();
		delete it->second;
		it->second = NULL;
	}
}

/// <summary>get the modelcount</summary>
/// <returns> return the modelcount as an integer </returns>
int ResourceManager::GetModelCount()
{
	return m_modelCount;
}

/// <summary>get the data (position and color) from the modellist with the given index number</summary>
/// <param name ="index"> the index in the array from where the color and position needs to be returned </param>
/// <param name ="position"> the position of the modellist with the given index  output</param>
/// <param name ="color"> the color of the moddellist with the given index  output</param>
void ResourceManager::GetData(int index,CustomVector3& position, CustomVector4& color)
{
	position = m_modelList[index].position;
	color = m_modelList[index].color;
}

/// <summary>initialize the modellist with the amount given as param set its position and color</summary>
/// <param name ="count"> how many models there should be placed </param>
/// <returns> succes yes/no? </returns>
bool ResourceManager::Initialize(int count)
{

	m_modelCount = count;

	m_modelList = new CustomColorType[m_modelCount];
	if(!m_modelList)
	{
		return false;
	}

	srand((unsigned int)time(NULL));

	for(int i = 0; i< m_modelCount; ++i)
	{
		m_modelList[i].color.t_x = (float)rand() / RAND_MAX;
		m_modelList[i].color.t_y = (float)rand() / RAND_MAX;
		m_modelList[i].color.t_z = (float)rand() / RAND_MAX;
		m_modelList[i].color.t_v = 1.0f; // alpha

		m_modelList[i].position.t_x = (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
		m_modelList[i].position.t_y = (((float)rand()-(float)rand())/RAND_MAX) * 10.0f;
		m_modelList[i].position.t_z = ((((float)rand()-(float)rand())/RAND_MAX) * 10.0f) + 5.0f;

	}
	return true;
}

/// <summary>load the texture at the given filepath as param, and its it to the texture list</summary>
/// <param name ="filePath"> the filepath of the texture </param>
/// <returns> succes yes/no </returns>
/// <seealso cref="Texture.Initialize()" />
bool ResourceManager::LoadTexture(std::string filePath)
{
	bool result;
	Texture* texture = new Texture();

	result = texture->Initialize(m_renderer,filePath);
	if(!result)
	{
		//loggermanager
		return false;
	}
	m_textures[filePath] = texture; // list with textures with the filepaths as key, we can call the actual texture from the renderer with this key.
	return true;
}

/// <summary>set the renderer</summary>
/// <param name ="renderer"> the renderer that will be used in the resourcemanager </param>
void ResourceManager::SetRenderer(I_Renderer* renderer)
{
	if(!m_renderer)
	{
		m_renderer = renderer;
	}
}

/// <summary>loads the model from a txt file</summary>
/// <param name ="filePath">the filepath of the model </param>
/// <param name ="vertexCount"> the vertexcount as output </param>
/// <param name ="indexCount"> the indexcount as output</param>
/// <returns>the model as a custommodeltype that holds the texture, position and normal values </returns>
CustomModelType* ResourceManager::LoadModel(std::string filePath, int& vertexCount, int&indexCount)
{
	std::ifstream fin;
	char input;
	 
	// Open the model file.
	fin.open(filePath);
	
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
	fin >> vertexCount;

	// Set the number of indices to be the same as the vertex count.
	indexCount = vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new CustomModelType[vertexCount];
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
	for(int i= 0; i< vertexCount; i++)
	{
		fin >> m_model[i].position.t_x >> m_model[i].position.t_y >> m_model[i].position.t_z;
		fin >> m_model[i].texture.t_x >> m_model[i].texture.t_y;
		fin >> m_model[i].normal.t_x >> m_model[i].normal.t_y >> m_model[i].normal.t_z;

		m_model[i].position.t_x = m_model[i].position.t_x + 130.0f;
		m_model[i].position.t_y = m_model[i].position.t_y + 18.0f;
		m_model[i].position.t_z = m_model[i].position.t_z + 140.0f;
	}

	// Close the model file.
	fin.close();

	return m_model;
}


