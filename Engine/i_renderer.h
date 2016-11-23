#ifndef _I_RENDERER_H_
#define _I_RENDERER_H_
 	
#include <windows.h>
#include <string>


#include "i_window.h"
#include "customstruct.h"
class I_Renderer
{
public:
	
	virtual bool Initialize(I_Window* window) = 0;
	virtual void Shutdown() = 0;
	virtual bool CreateDevice() = 0;
	virtual void Frame(int indexCount, std::string shaderName) = 0; 
	
	virtual void ClearDepthStencil(std::string identifier) = 0;
	virtual void ClearTargetRenderView(float rgba[], std::string identifier) = 0;
	virtual void LockRefreshRate(std::string identifier) = 0;
	
	virtual bool CreateModel(CustomModelType* vertices, int verticesCount, unsigned long* indices, int indicesCount,  std::string identifier) = 0;
	virtual bool CreateTerrain(CustomVertexType* vertices, int verticesCount, unsigned long* indices, int indicesCount, std::string identifier) = 0;
	virtual bool CreateSkydome(CustomVector3* vertices, int verticesCount, unsigned long* indices, int indicesCount, std::string identifier) = 0;
	virtual bool CreateTexture(std::string textureName) = 0;
	
	virtual void TurnOffCulling() = 0;
	virtual void TurnOnCulling() = 0;
	virtual void TurnOffZBuffer()= 0;
	virtual void TurnOnZBuffer()= 0;

	virtual bool SetupShader(std::string identifier, CustomMatrix view, CustomMatrix world, CustomMatrix projection, CustomLightBufferType lightbuffer, std::string shaderName) = 0;
	virtual bool SetupSkydomeShader(CustomMatrix view, CustomMatrix world, CustomMatrix projection, CustomGradientBufferType gradientColor, std::string shaderName) = 0;

	virtual void SetupBuffer(unsigned int stride, unsigned int offset, std::string identifier) = 0;

	virtual bool CreateShader(std::string vsFilename,std::string psFilename,std::string shaderName) = 0;
	virtual	bool CreateSkydomeShader(std::string vsFilename, std::string psFilename,std::string shaderName) = 0;

	virtual void GetProjectionMatrix(CustomMatrix& projection, std::string identifier) = 0;
	virtual void GetWorldMatrix(CustomMatrix& world, std::string identifier) = 0;
	//virtual void GetOrthoMatrix(CustomMatrix& ortho, std::string identifier) = 0;
	
	virtual void RotationMatrix(float rotation, std::string identifier, CustomMatrix& matrix)= 0;
	virtual void TranslateMatrix(CustomVector3 position, std::string identifier, CustomMatrix& matrix) = 0;

};

#endif