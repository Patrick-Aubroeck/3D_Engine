#include "light.h"

/// <summary> standard constructor </summary>
Light::Light() : 
	m_lighting()
{

}

/// <summary> standard destructor call the shutdown function instead </summary>
Light::~Light()
{

}

/// <summary>set the diffusecolor of the lighting</summary>
/// <param name ="diffuseColor"> the color of the lighting  in red green blue alpha colors</param>
void Light::SetDiffuseColor(CustomVector4 diffuseColor)
{
	m_lighting.diffuseColor = diffuseColor;
}

/// <summary>set the direction of the lighting</summary>
/// <param name ="direction"> the direction in a x y z coordinate that the lighting should face at </param>
void Light::SetDirection(CustomVector3 direction)
{
	m_lighting.lightDirection = direction;
}

/// <summary>get the diffusecolor</summary>
/// <returns> returns the diffuse color as a customvector 4 with rgba values in it</returns>
CustomVector4 Light::GetDiffuseColor()
{
	return m_lighting.diffuseColor;
}

/// <summary>get the direction of the lighting</summary>
/// <returns>returns the direction of the lighting as a customvector 3 with x y z coordinates </returns>
CustomVector3 Light::GetDirection()
{
	return m_lighting.lightDirection;
}
/// <summary>get the lighting values diffuse ambient and direction</summary>
/// <returns> return the lighting values  as a customlightbuffer struct</returns>
CustomLightBufferType Light::GetLighting()
{
	return m_lighting;
}

/// <summar> shutdown the light and call the shutdown function of its member variables </summary>
void Light::Shutdown()
{
	//nothing to do for now
}

/// <summary> set the ambient color</summary>
/// <param name ="ambientColor"> the ambient color as a customvector 4 , with  rgba values </param>
void Light::SetAmbientColor(CustomVector4 ambientColor)
{
	m_lighting.ambientColor = ambientColor;
}
/// <summary>get the ambient color as a customvector4</summary>
/// <returns> returns the ambient color as a customvector 4 with rgba values</returns>
CustomVector4 Light::GetAmbientColor()
{
	return m_lighting.ambientColor;
}



