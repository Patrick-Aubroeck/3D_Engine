#ifndef _SHADER_H_
#define _SHADER_H_

// standard library includes
#include "i_renderer.h"
#include "i_window.h"

class Shader
{
private:
	std::string m_vsFileName; //the vsfilepath
	std::string m_psFileName; //the psfilepath
public:
	Shader(); // standard constructor
	~Shader(); // standard destructor
	bool Initialize(I_Renderer* renderer,std::string shaderName); // initialize the shader
	void Shutdown(); // shutdown the shader and its member variable
	bool Render(I_Renderer* renderer, int indexCount, std::string textureName, CustomMatrix view, CustomMatrix world, CustomMatrix projection, CustomLightBufferType lightBuffer,  std::string shaderName); // set the vertex and index buffer ready for the graphic pipeline and draw them 
	bool InitializeSkybox(I_Renderer* renderer,std::string shaderName); // initialize the skybox
	bool RenderSkybox(I_Renderer* renderer, int indexCount, CustomMatrix view, CustomMatrix world, CustomMatrix projection, CustomGradientBufferType gradientColor,  std::string shaderName); // set the skybox vertex and index buffer ready for the graphic pipeline and draw them
	std::string GetHLSLVsFile(); // get the vs filepath
	std::string GetHLSLPsFile(); // get the ps filepath
	void SetHLSLVsFileName(std::string filename); // set the vs filepath
	void SetHLSLPsFileName(std::string filename); // set the ps filepath
private:
	bool InitializeShader(I_Renderer* renderer,std::string shaderName); // create the shader 
	bool SetShaderParameters(I_Renderer* renderer, std::string textureName, CustomMatrix view, CustomMatrix world, CustomMatrix projection , CustomLightBufferType lightbuffer, std::string shaderName); // set the vertex and index buffer ready for the graphic pipeline
	void RenderShader(I_Renderer* renderer, int indexCount, std::string shaderName); // draw the input 
	bool SetSkyboxShaderParameters(I_Renderer* renderer, CustomMatrix view, CustomMatrix world, CustomMatrix projection, CustomGradientBufferType gradientBuffer, std::string shaderName); // set the skybox vertex and index buffer ready for the graphic pipeline
};

#endif