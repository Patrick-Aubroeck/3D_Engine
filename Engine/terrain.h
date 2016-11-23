#ifndef _TERRAIN_H_
#define _TERRAIN_H_

//standard include
#include <stdio.h>

//own include
#include "i_renderer.h"
#include "resourcemanager.h"
#include "customstruct.h"



class Terrain
{
private:
	int m_terrainWidth; // holds the terrainwidth
	int m_terrainHeight; // holds the terrainheight
	int m_vertexCount; // holds the verticescount
	int m_indiceCount; //holds the indicescount
	CustomVertexType* m_heightMap; //holds the position, texture, color and normal values of the terrain
	const int m_textureRepeat; // the repeat value of the texture

public:
	Terrain(); // standard constructor
	virtual ~Terrain(); // standard destructor
	bool Initialize(I_Renderer* renderer, ResourceManager* resourceManager, std::string terrainFileName, std::string textureFileName, std::string colorFileName, float normalizer); // initialize the buffers of the terrain with all the information it needs
	void Shutdown(); // shutdown the member variables 
	void Render(I_Renderer* renderer, std::string terrainTexturename); // set the buffers ready for the graphic pipeline and draw
	int GetIndexCount(); // get the indicecount
private:
	void CalculateTextureCoordinates(); // calculate the texturecoordinates using the texturerepeat
	bool LoadHeightMap(std::string terrainFileName); // load in the heightmap from a bmp file
	void NormalizeHeightMap(float normalizer); // normalize the heightmap to make it look more realistic
	bool LoadTexture(ResourceManager* resourceManager,std::string textureFileName); // load the texture using the resourcemanager
	bool CalculateNormals(); // calculate the normals of the terrain
	bool LoadColorMap(ResourceManager* resourceManager,std::string colorFileName); // load in the color using the resourcemanager
	void ShutdownHeightMap(); // shutdown the m_heightmap 
	bool InitializeBuffers(I_Renderer* renderer, std::string terrainTexturename); // initialize the buffers with all the information needed
	void RenderBuffers(I_Renderer* renderer, std::string terrainTexturename); // set the buffers ready for the graphic pipeline
};

#endif