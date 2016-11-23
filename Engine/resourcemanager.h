#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

//standard includes
#include <fstream>
#include <map>
#include <vector>

//own includes
#include "i_renderer.h"
#include "resourcemodel.h"
#include "texture.h"
#include "shader.h"

class ResourceManager
{
private:
	int m_modelCount; // holds the modelcount
	CustomColorType* m_modelList; //container to hold the color and position of the models
	CustomModelType* m_model; // container to hold the position texture and normal
	std::map <std::string, Texture*> m_textures; // container to hold the texture of a model with a string(texturename) as key
	I_Renderer* m_renderer; // holds the renderer
public:
	ResourceManager(); //constructor
	~ResourceManager(); // destructor
	void Shutdown(); //calls the child shutdown function
	bool Initialize(int count); // creates a model ( the param is for the amount)
	bool LoadTexture(std::string filePath); // load a texture with the given texture name
	CustomModelType* LoadModel(std::string filePath, int& indexCount, int& vertexCount); // returns a position texture and normal of a model, sets the index and vertex count of a model
	void SetRenderer(I_Renderer* renderer); // sets the renderer
	int GetModelCount(); //returns the modelCount
	void GetData(int index, CustomVector3& position, CustomVector4& color); // returns the position and color of the model at the given index

};
#endif