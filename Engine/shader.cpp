#include "shader.h"

///<summary> standard constructor</summary>
Shader::Shader() :
	m_vsFileName(""),
	m_psFileName("")
{

}

///<summary> standard destructor </summary>
Shader::~Shader()
{

}

/// <summary>Initialize the shader by calling initialize shader</summary>
/// <param name ="renderer"> the rendere that has to call the draw </param>
/// <param name ="shaderName">the name of the shader </param>
/// <returns> succes yes/no?</returns>
/// <seealso cref="InitializeShader()" />
bool Shader::Initialize(I_Renderer* renderer,std::string shaderName)
{
	bool result;

	result = InitializeShader(renderer, shaderName);
	if(!result)
	{
		return false;
	}

	return true;
}

/// <summary>shutdown the shader and its member variables</summary>
void Shader::Shutdown()
{
	// nothing to shutdown for now
}

/// <summary>render the shader by calling the setshaderparameters and rendershader</summary>
/// <param name ="renderer"> the renderer that needs to draw </param>
/// <param name ="indexCount"> the indexcount </param>
/// <param name ="textureName">  the path of the texture </param>
/// <param name ="view"> the view matrix </param>
/// <param name ="world"> the world matrix </param>
/// <param name ="projection"> the projection matrix </param>
/// <param name ="lightbuffer"> the lightbuffer that holds the diffuse, direction and ambientcolor</param>
/// <param name ="shaderName"> the name of the shader </param>
/// <returns> succes yes/no? </returns>
/// <seealso cref="SetShaderParameters()" />
/// <seealso cref="RenderShader()" />
bool Shader::Render(I_Renderer* renderer, int indexCount, std::string textureName, CustomMatrix view, CustomMatrix world, CustomMatrix projection, CustomLightBufferType lightbuffer,  std::string shaderName)
{
	bool result;
	//setup the shader 
	result = SetShaderParameters(renderer, textureName, view, world, projection , lightbuffer , shaderName);
	if(!result)
	{
		return false;
	}
	// draw the shader
	RenderShader(renderer,indexCount, shaderName);

	return true;
}

/// <summary> initialize the shader</summary>
/// <param name ="renderer"> the renderer that calls the createshader</param>
/// <param name ="shaderName"> the name of the shader </param>
/// <returns> succes yes/no? </returns>
/// <seealso cref="Renderer.CreateShader()" />
bool Shader::InitializeShader(I_Renderer* renderer,std::string shaderName)
{
	renderer->CreateShader(m_vsFileName,m_psFileName,shaderName);
	return true;
}

/// <summary>set the shader ready for the graphic pipeline</summary>
/// <param name ="renderer"> the renderer that has to call the setupshader function </param>
/// <param name ="identifier"> the path of the file that is used as identifier </param>
/// <param name ="view"> the viewmatrix </param>
/// <param name ="world"> the world matrix </param>
/// <param name ="projection"> the projection matrix </param>
/// <param name ="lightbuffer"> the lightbuffer that holds the diffuse,direction and ambientcolor </param>
/// <param name ="shaderName"> the name of the shader </param>
/// <returns> succes yes/no? </returns>
/// <seealso cref="Renderer.SetupShader()" />
bool Shader::SetShaderParameters(I_Renderer* renderer, std::string identifier, CustomMatrix view, CustomMatrix world, CustomMatrix projection, CustomLightBufferType lightbuffer, std::string shaderName)
{
	bool result;
	result = renderer->SetupShader(identifier, view, world,projection, lightbuffer, shaderName);
	if(!result)
	{
		return false;
	}
	return true;
}

/// <summary>draw the shader</summary>
/// <param name ="renderer">the renderer that has to call the frame function </param>
/// <param name ="indexCount"> the indexcount </param>
/// <param name ="shaderName"> the name of the shader </param>
/// <seealso cref="Renderer.Frame()"/> 
void Shader::RenderShader(I_Renderer* renderer, int indexCount,  std::string shaderName)
{
	renderer->Frame(indexCount, shaderName);
}

/// <summary>get the vs file</summary>
/// <returns> the path of the file as string</returns>
std::string Shader::GetHLSLVsFile()
{
	return m_vsFileName;
}

/// <summary>get the ps file</summary>
/// <returns>the path of the file as string </returns>
std::string Shader::GetHLSLPsFile()
{
	return m_psFileName;
}

/// <summary>set the vs file</summary>
/// <param name ="filename"> the path of the file </param>
void Shader::SetHLSLVsFileName(std::string filename)
{
	m_vsFileName = filename;
}

/// <summary>get the ps file</summary>
/// <param name ="filename"> the path of the file</param>
void Shader::SetHLSLPsFileName(std::string filename)
{
	m_psFileName = filename;
}

/// <summary>get the ps file</summary>
/// <param name ="renderer"> then renderer that has to call the createskydomeshader</param>
/// <param name ="shaderName"> the name of the shader</param>
/// <return> succes yes/no? </return>
/// <seealso cref="Renderer.CreateSkydomeShader()"/> 
bool Shader::InitializeSkybox(I_Renderer* renderer,std::string shaderName)
{
	renderer->CreateSkydomeShader(m_vsFileName,m_psFileName,shaderName);
	return true;
}

/// <summary>setup the shader ready for the graphic pipeline</summary>
/// <param name ="renderer"> then renderer that has to call the createskydomeshader</param>
/// <param name ="view"> the view matrix</param>
/// <param name ="world"> the world matrix</param>
/// <param name ="projection"> the projection matrix </param>
/// <param name ="gradientColor"> the gradient color that holds the apex and center color</param>
/// <param name ="shaderName"> the name of the shader</param>
/// <return> succes yes/no? </return>
/// <seealso cref="Renderer.SetupSkydomeShader()"/> 
bool Shader::SetSkyboxShaderParameters(I_Renderer* renderer, CustomMatrix view, CustomMatrix world, CustomMatrix projection , CustomGradientBufferType gradientColor, std::string shaderName)
{
	renderer->SetupSkydomeShader( view, world, projection, gradientColor, shaderName);
	return true;
}

/// <summary>get the ps file</summary>
/// <param name ="renderer"> then renderer that has to call the createskydomeshader</param>
/// <param name ="indexCount"> the indexCount</param>
/// <param name ="view"> the view matrix</param>
/// <param name ="world"> the world matrix</param>
/// <param name ="projection"> the projection matrix </param>
/// <param name ="gradientColor"> the gradientColor that holds the apex and center color</param>
/// <param name ="shaderName"> the name of the shader</param>
/// <return> succes yes/no? </return>
/// <seealso cref="Renderer.SetSkyboxShaderParameters()"/> 
/// <seealso cref="Renderer.Frame()"/> 
bool Shader::RenderSkybox(I_Renderer* renderer, int indexCount, CustomMatrix view, CustomMatrix world, CustomMatrix projection, CustomGradientBufferType gradientColor, std::string shaderName)
{
	bool result;

	result = SetSkyboxShaderParameters(renderer, view, world, projection , gradientColor, shaderName);
	if(!result)
	{
		return false;
	}

	renderer->Frame(indexCount, shaderName);

	return true;
}