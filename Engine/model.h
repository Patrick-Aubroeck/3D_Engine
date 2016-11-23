#ifndef _MODEL_H_
#define _MODEL_H_

// own includes
#include "entity.h"
#include "texture.h"
#include "resourcemanager.h"

class Model : public Entity
{
private:
	int m_vertexCount; // holds the vertexCount
	unsigned long* m_indices; // holds the indiceCount
	CustomVertexType* m_vertices; //holds the position , texture, normal color of the model
	CustomModelType* m_model; //holds the position, texture, normal of each vertice

public:	
	Model(); // constructor
	virtual ~Model(); // destructor
	bool Initialize(I_Renderer* renderer, ResourceManager* resourceManager); // creates a model
	void Shutdown(); // shutdown the class and its child classes
	void Render(I_Renderer* renderer, std::string identifier); // render the buffers and set them ready for drawing

	int GetVertexCount(); // returns the vertexCount of the model
	unsigned long* GetIndices(); // returns the indiceCount of the model
	CustomVertexType* GetVertices(); // returns the vertexType ( position , texture , normal ) values
	CustomModelType* GetModel(); // returns the modeltype of this model
private:
	bool InitializeBuffers(I_Renderer* renderer,std::string identifier); // setup the buffers and set the ready for the vertex and pixel shader
	void RenderBuffers(I_Renderer* renderer, std::string identifier); // render the buffers and set them ready for drawing
};

#endif