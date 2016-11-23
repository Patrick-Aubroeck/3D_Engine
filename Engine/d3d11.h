#ifndef _D3D11_H_
#define _D3D11_H_

// Standard Library Includes
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <map>
#include <string>
#include <vector>

// Own Includes
#include "i_renderer.h"
#include "window.h"
#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\D3DX11async.h"
#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\D3DX10math.h"


class D3D11 : public I_Renderer
{
private:
	struct CustomMatrixStruct
	{
		D3DXMATRIX t_world;
		D3DXMATRIX t_view;
		D3DXMATRIX t_projection;
		//D3DXMATRIX t_ortho; // used for 2d rendering 
	}; // a struct to hold the 3 d3dxmatrixen the renderer is going to use

	struct DrawInterfaces
	{
		 IDXGISwapChain* t_swapchain;
		 ID3D11RenderTargetView* t_renderTargetView;
		 ID3D11DepthStencilView* t_depthStencilView;
	};  // a struct to hold the interfaces needed to draw (for each window)

	struct BufferStruct
	{
		ID3D11Buffer* t_vertexBuffer;
		ID3D11Buffer* t_indiceBuffer;
	}; // vertex and indice buffer

	struct ShaderStruct
	{
		ID3D11VertexShader* t_vertexShader;
		ID3D11PixelShader* t_pixelShader;
		ID3D11InputLayout* t_layout;
		ID3D11Buffer* t_matrixBuffer;
		ID3D11Buffer* t_lighting;
		ID3D11Buffer* t_gradientBuffer;
	}; // a struct that holds all the shader materials


	int m_videoCardMemory; // store the graphics card capacity
	char m_videoCardDescription[128]; // store all the information of the graphic card
	ID3D11SamplerState* m_sampleState; 
	IDXGIFactory* m_factory; // graphic card interface variable (used to create the swapchain)
	ID3D11Device* m_device; // a pointer to the device
	ID3D11DeviceContext* m_deviceContext; // a pointer to the device context
	IDXGIAdapter* m_adapter; //represents the display system
	IDXGIOutput* m_adapterOutput; //represents the monitor

	std::map<std::string, ID3D11ShaderResourceView*> m_texture; // container to hold the texture
	std::map<std::string, CustomMatrixStruct*> m_matrixen; // the combination of a world, view, projection and ortho matrix in a struct.
	std::map<std::string, ShaderStruct*> m_shaders; // a map to keep a combination of a shaderstruct and a string name 
	std::map<std::string, BufferStruct*> m_buffers; // the vertex and indice buffer of the specified model(string name)
	std::map<std::string, DrawInterfaces*> m_drawInterfaces; // a render target view interface .. holds the render target subresources

	ID3D11Texture2D* m_depthStencilBuffer; //2D texture interface
	ID3D11DepthStencilState* m_depthStencilState; // holds a description for the depth stencil state
	ID3D11RasterizerState* m_rasterState; // holds a description of the rasterizer state
	ID3D11RasterizerState* m_rasterStateNoCulling; // holds a description to the rasterstate where culling is set on None
	ID3D11DepthStencilState* m_depthDisabledStencilState; //holds a description to the depthstencilstate where the depthstencildepth is disabled
public:
	D3D11(); // standard constructor
	virtual ~D3D11(); // standard destructor
	bool Initialize(I_Window* window); // initialize a swapchain, depthbuffer, depthstencil, rasterdesc and viewport for the window given as parameter
	void Shutdown(); // shutdown the D3D11Device
	bool CreateDevice(); // creates a D3D11Device and D3D11Device context
	void Frame(int indexCount, std::string shaderName); // set the pixel and vertex shader ready to be drawn, and call the draw 

	void ClearTargetRenderView(float rgba[], std::string identifier); // calls the clearrendertargetview for the identifier given as parameter
	void ClearDepthStencil(std::string identifier); //calls the cleardepthstencil 
	void LockRefreshRate(std::string identifier); // presents the backbuffer to the frontbuffer

	bool CreateModel(CustomModelType* vertices, int verticesCount, unsigned long* indices, int indicesCount, std::string identifier); //creates a buffer for the model
	bool CreateTerrain(CustomVertexType* vertices, int verticesCount, unsigned long* indices, int indicesCount, std::string identifier); // creates the terrain buffers
	bool CreateSkydome(CustomVector3* vertices, int verticesCount, unsigned long* indices, int indicesCount, std::string identifier); // create the skydomebuffers
	bool CreateTexture(std::string textureName); // creates a texture with the given texture name and holds it in a container
	
	void TurnOffCulling();
	void TurnOnCulling();
	void TurnOffZBuffer();
	void TurnOnZBuffer();

	bool SetupShader(std::string identifier, CustomMatrix view, CustomMatrix world, CustomMatrix projection, CustomLightBufferType lightbuffer, std::string shaderName); // setup the shaders
	bool SetupSkydomeShader(CustomMatrix view, CustomMatrix world, CustomMatrix projection, CustomGradientBufferType gradientColor, std::string shaderName);

	void SetupBuffer(unsigned int stride, unsigned int offset, std::string identifier); // set up the buffer 

	bool CreateShader(std::string vsFilename, std::string psFilename,std::string shaderName); // create a shader with a vertex and pixelshader
	bool CreateSkydomeShader(std::string vsFilename, std::string psFilename,std::string shaderName);
	
	void GetProjectionMatrix(CustomMatrix& projection, std::string identifier); // returns the ProjectionMatrix
	void GetWorldMatrix(CustomMatrix& world, std::string identifier); // returns the WorldMatrix
	//void GetOrthoMatrix(CustomMatrix& ortho, std::string identifier); // returns the OrthoMatrix
	
	void TranslateMatrix(CustomVector3 position, std::string identifier, CustomMatrix& matrix); // setup the world matrix
	void RotationMatrix(float rotation, std::string identifier, CustomMatrix& matrix); // updates the world rotation by changing its rotation value
	
private:
	void SetVideoCardInfo(char* cardName, int& memory); // set the video card info
	void GetTexture(std::string textureName); //  returns the texture with the name given as param (if it exist) return null otherwise
	const int GetVideoCardMemory(); // returns the graphiccards memory
	const char* GetVideoCardDescription(); // returns the information of the graphic card
	
	void CreateD3DXMatrix(D3DXMATRIX& d3dXMatrix, CustomMatrix matrix); // Creates a d3dxmatrix from the custommatrix

};

#endif