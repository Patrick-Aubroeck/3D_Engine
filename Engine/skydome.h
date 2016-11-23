#ifndef _SKYDOME_H_
#define _SKYDOME_H_

//standard library includes
#include <fstream>

// own includes
#include "i_renderer.h"
#include "customstruct.h"

class Skydome
{
private:
	CustomModelType* m_model; //holds the position texture and normal values of the skydome per vertex
	int m_vertexCount; // amount of vertexes
	int m_indiceCount; // amount of indices
	CustomGradientBufferType m_gradientColor; //holds the apex and centerColor of the skydome per vertex
public:
	Skydome(); //standard constructor
	virtual ~Skydome(); //standard destructor
	bool Initialize(I_Renderer* renderer, std::string skyboxTextureName); // initialize the skydome buffer with all the information needed
	void Shutdown(); // call the shutdownheightmap function
	void Render(I_Renderer* renderer, std::string skyboxTexturename); // set the buffers ready for the graphic pipeline
	int GetIndexCount(); // get the indexcount
	CustomGradientBufferType GetGradientColor(); // get the gradientcolor (apex and center)
	void SetGradientColor(CustomGradientBufferType gradientColor); // set the gradientcolor

private:
	bool LoadSkydomeModel(std::string skydomeFileName); // load in the model of the skydome
	bool InitializeBuffers(I_Renderer* renderer, std::string skyboxTexturename); // initialize the buffers with all the information needed
	void RenderBuffers(I_Renderer* renderer, std::string skyboxTexturename); // set the buffers ready for the graphic pipeline

};

#endif