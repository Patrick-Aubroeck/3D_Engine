#ifndef _LIGHT_H_
#define _LIGHT_H_

//own includes
#include "customstruct.h"

class Light
{
private:
	CustomLightBufferType m_lighting; // container to hold the ambient, diffuse and direction 
public:
	Light(); //constructor
	~Light(); // destructor
	void SetDiffuseColor(CustomVector4 diffuseColor); //sets the "sun" lighting
	void SetDirection(CustomVector3 direction); // sets the direction the diffusecolor is pointing at
	void SetAmbientColor(CustomVector4 ambientColor); // sets the surrounding lighting
	void Shutdown();
	CustomVector4 GetAmbientColor(); //returns the surrounding lighting color
	CustomVector4 GetDiffuseColor(); //returns the "sun" lighting color
	CustomVector3 GetDirection(); // returns the direction the lighting color was pointing at
	CustomLightBufferType GetLighting(); // returns the ambient,diffuse and direction of the light class

};

#endif