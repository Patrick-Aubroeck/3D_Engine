#ifndef _ENTITY_H_
#define _ENTITY_H_

// own includes
#include "i_renderer.h"
#include "resourcemanager.h"

class Entity 
{
protected:
	int m_indiceCount; // holds the indiceCount
public:
	virtual bool Initialize(I_Renderer* renderer, ResourceManager* resourceManager) = 0;
	virtual	void Render(I_Renderer* renderer, std::string identifier) = 0;
	virtual int GetIndiceCount(); // returns the indiceCount
	virtual void Shutdown() = 0;

};

#endif