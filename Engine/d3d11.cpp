#include "d3d11.h"
#include "settings.h"
#include "shader.h"

D3D11::D3D11() :
	m_device(NULL),
	m_deviceContext(NULL),
	m_depthStencilBuffer(NULL),
	m_depthStencilState(NULL),
	m_rasterState(NULL),
	m_adapter(NULL),
	m_adapterOutput(NULL),
	m_buffers(),
	m_depthDisabledStencilState(NULL),
	m_drawInterfaces(),
	m_factory(NULL),
	m_matrixen(),
	m_rasterStateNoCulling(NULL),
	m_sampleState(NULL),
	m_shaders(),
	m_texture(),
	m_videoCardDescription(),
	m_videoCardMemory(0)
{
	
}

D3D11::~D3D11()
{

}
/// <summary>calls the shutdown and release function of all the member variable.Sets the variable to NULL after releasing them</summary>
void D3D11::Shutdown()
{
	//Before releasing set to windowed mode ,oterhwise when you release the swap chain it will throw an exception.
	for(std::map<std::string, DrawInterfaces*>::iterator it = m_drawInterfaces.begin(); it!= m_drawInterfaces.end(); ++it) 
	{	
		it->second->t_swapchain->SetFullscreenState(false,NULL);
		it->second->t_swapchain->Release();
		it->second->t_renderTargetView->Release();
		it->second->t_depthStencilView->Release();
	}
	//Shut down the vertex and indiceBuffer
	for(std::map<std::string, BufferStruct*>::iterator it = m_buffers.begin(); it!= m_buffers.end(); ++it) 
	{
		it->second->t_indiceBuffer->Release();
		it->second->t_indiceBuffer = NULL;
		it->second->t_vertexBuffer->Release();
		it->second->t_vertexBuffer = NULL;
	}
	for(std::map<std::string, ShaderStruct*>::iterator it = m_shaders.begin(); it!= m_shaders.end(); ++it) 
	{
		it->second->t_pixelShader->Release();
		it->second->t_pixelShader = NULL;
		it->second->t_vertexShader->Release();
		it->second->t_vertexShader = NULL;
		it->second->t_matrixBuffer->Release();
		it->second->t_matrixBuffer = NULL;
		it->second->t_layout->Release();
		it->second->t_layout = NULL;
		if(it->second->t_gradientBuffer)
		{
			it->second->t_gradientBuffer->Release();
			it->second->t_gradientBuffer = NULL;
		}
		if(it->second->t_lighting)
		{
			it->second->t_lighting->Release();
			it->second->t_lighting = NULL;
		}
	}
	for(std::map<std::string, ID3D11ShaderResourceView*>::iterator it = m_texture.begin(); it!= m_texture.end(); ++it) 
	{
		it->second->Release();
		it->second = NULL;
	}
	for(std::map<std::string, CustomMatrixStruct*>::iterator it = m_matrixen.begin(); it!= m_matrixen.end(); ++it) 
	{
		delete it->second;
		it->second = NULL;
	}

	if(m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = NULL;
	}
	if(m_rasterStateNoCulling)
	{
		m_rasterStateNoCulling->Release();
		m_rasterStateNoCulling = NULL;
	}

	if(m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = NULL;
	}
	if(m_depthDisabledStencilState)
	{
		m_depthDisabledStencilState->Release();
		m_depthDisabledStencilState = NULL;
	}
	if(m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = NULL;
	}
	if(m_adapterOutput)
	{
		m_adapterOutput->Release();
		m_adapterOutput = NULL;
	}

	if(m_adapter)
	{
		m_adapter->Release();
		m_adapter = NULL;
	}

	if(m_factory)
	{
		m_factory->Release();
		m_factory = NULL;
	}
	if(m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = NULL;
	}
	if(m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = NULL;
	}
	if(m_device)
	{
		m_device->Release();
		m_device = NULL;
	}
}

/// <summary>call this to clear the depthstencilview</summary>
/// <param name ="identifier"> the name of the window you want to call the clear function on </param>
void D3D11::ClearDepthStencil(std::string identifier)
{
	m_deviceContext->ClearDepthStencilView(m_drawInterfaces[identifier]->t_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

/// <summary> will clear the backbuffer with the given rgba value given as param</summary>
/// <param name ="identifier"> the name of the window you want to call the clearn function on </param>
/// <param name ="rgba"> the color values red, green, blue and alpha </param>
void D3D11::ClearTargetRenderView(float rgba[],std::string identifier)
{
	m_deviceContext->ClearRenderTargetView(m_drawInterfaces[identifier]->t_renderTargetView,rgba);
}

/// <summary>will present on the given window</summary>
/// <param name ="identifier"> the windowname you want to present on </param>
/// <remark>if Vsync is enabled will present according to the window refreshrate if Vsync is disabled will present as fast as possible
/// WARNING turning of vsync could result into tearing WARNING</remark>
void D3D11::LockRefreshRate(std::string identifier)
{
			if(Settings::VSYNC_ENABLED)
			{
				// Lock to screen refresh rate.
				m_drawInterfaces[identifier]->t_swapchain->Present(1, 0);
			}
			else
			{
				// Present as fast as possible.
				m_drawInterfaces[identifier]->t_swapchain->Present(0, 0);
			}
}

/// <summary>get the projectionMatrix</summary>
/// <param name ="identifier"> the windowname of the window that needs to return its projectionmatrix</param>
/// <param name ="projection"> the return value as a customMatrix </param>
void D3D11::GetProjectionMatrix(CustomMatrix& projection, std::string identifier)
{
	projection = CustomMatrix(m_matrixen[identifier]->t_projection._11, m_matrixen[identifier]->t_projection._12, m_matrixen[identifier]->t_projection._13, m_matrixen[identifier]->t_projection._14,
							  m_matrixen[identifier]->t_projection._21, m_matrixen[identifier]->t_projection._22, m_matrixen[identifier]->t_projection._23, m_matrixen[identifier]->t_projection._24,
							  m_matrixen[identifier]->t_projection._31, m_matrixen[identifier]->t_projection._32, m_matrixen[identifier]->t_projection._33, m_matrixen[identifier]->t_projection._34,
							  m_matrixen[identifier]->t_projection._41, m_matrixen[identifier]->t_projection._42, m_matrixen[identifier]->t_projection._43, m_matrixen[identifier]->t_projection._44);
}

/// <summary>get the worldMatrix</summary>
/// <param name ="identifier"> the windowname of the window that needs to return its worldmatrix</param>
/// <param name ="projection"> the return value as a customMatrix </param>
void D3D11::GetWorldMatrix(CustomMatrix& world, std::string identifier)
{
	world = CustomMatrix(m_matrixen[identifier]->t_world._11, m_matrixen[identifier]->t_world._12, m_matrixen[identifier]->t_world._13, m_matrixen[identifier]->t_world._14,
						 m_matrixen[identifier]->t_world._21, m_matrixen[identifier]->t_world._22, m_matrixen[identifier]->t_world._23, m_matrixen[identifier]->t_world._24,
						 m_matrixen[identifier]->t_world._31, m_matrixen[identifier]->t_world._32, m_matrixen[identifier]->t_world._33, m_matrixen[identifier]->t_world._34,
						 m_matrixen[identifier]->t_world._41, m_matrixen[identifier]->t_world._42, m_matrixen[identifier]->t_world._43, m_matrixen[identifier]->t_world._44);
}

/// <summary>get the memory in mb of the graphic card</summary>
/// <return> returns the memory in mb of the graphic card</return>
const int D3D11::GetVideoCardMemory()
{
	return m_videoCardMemory;
}

/// <summary>get the graphic card information</summary>
/// <return> return the graphic card information as a char array </return>
const char* D3D11::GetVideoCardDescription()
{
	return m_videoCardDescription;
}

/// <summary> set the graphic card information</summary>
/// <param name ="cardName"> the graphic card name</param>
/// <param name ="memory"> the output amount of memory in mb</param>
void D3D11::SetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
}


/// <summary>Initialize a Adapter, AdapterOutput, Swapchain, Depthbuffer, DepthStencil and Viewport</summary>
/// <param name ="window"> The window which you want to Initialize the directx to </param>
/// <returns> succes yes/no?</returns>
/// <remark> call this function for each window you want to Initialize a directx device to </remark>
bool D3D11::Initialize(I_Window* window)
{
	
	HRESULT result;
	unsigned int numerator;
	unsigned int denominator;
	unsigned int numModes; 
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView;
	float screenAspect;
	DXGI_MODE_DESC* displayModeList;
	IDXGISwapChain* swapChain;
	ID3D11Texture2D* backBufferPtr;

	D3DXMATRIX projectionMatrix; //struct holds a 4x4 matrix
	D3DXMATRIX worldMatrix; //struct holds a 4x4 matrix
	D3DXMATRIX orthoMatrix; //struct holds a 4x4 matrix

	CustomMatrixStruct* matrix = new CustomMatrixStruct();
	DrawInterfaces* drawInterfaces = new DrawInterfaces();
	
	int screenWidth = window->GetScreenWidth();
	int screenHeight = window->GetScreenHeight();
	

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	result = m_adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(result))
	{
		return false;
	}
	
	// Create a list to hold all the possible display modes for this monitor/video card combination.
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
	{
		return false;
	}
	// Now fill the display mode list structures.
	result = m_adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(result))
	{
		return false;
	}

	// Now go through all the display modes and find the one that matches the screen width and height.
	// When a match is found store the numerator and denominator of the refresh rate for that monitor.
	for(unsigned int i = 0; i<numModes; i++)
	{
		if(displayModeList[i].Width == (unsigned int)screenWidth)
		{
			if(displayModeList[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
				break;
			}
		}
	}

	
	// Release the display mode list.
	delete [] displayModeList;
	displayModeList = NULL;

	// Initialize the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if(Settings::VSYNC_ENABLED)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
	swapChainDesc.OutputWindow = dynamic_cast<Window*>(window)->GetWindowHandleInstance();

	// Turn multisampling off.
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if(window->GetFullScreenSetting())
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	m_factory->CreateSwapChain(m_device,&swapChainDesc,&swapChain);

	// Get the pointer to the back buffer.
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if(FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if(FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if(FAILED(result))
	{
		return false;
	}

	// Setup a raster description which turns off back face culling.
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the no culling rasterizer state.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterStateNoCulling);
	if(FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);
	
	// Setup the viewport for rendering.
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &viewport);

	// Setup the projection matrix.
	fieldOfView = (float)D3DX_PI / 4.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, fieldOfView, screenAspect, Settings::NEARCLIPPING, Settings::FARCLIPPING);

	// Initialize the world matrix to the identity matrix.
	D3DXMatrixIdentity(&worldMatrix);

	// Create an orthographic projection matrix for 2D rendering.
	D3DXMatrixOrthoLH(&orthoMatrix, (float)screenWidth, (float)screenHeight,  Settings::NEARCLIPPING, Settings::FARCLIPPING);

	// Clear the second depth stencil state before setting the parameters.
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
	if(FAILED(result))
	{
		return false;
	}


	drawInterfaces->t_swapchain = swapChain;
	drawInterfaces->t_renderTargetView = renderTargetView;
	drawInterfaces->t_depthStencilView = depthStencilView;
	m_drawInterfaces[window->GetWindowName()] = drawInterfaces;

	matrix->t_projection = projectionMatrix;
	matrix->t_world = worldMatrix;
//	matrix->t_ortho = orthoMatrix;
	m_matrixen[window->GetWindowName()]= matrix;
	return true;
}

/// <summary>setup the buffer for rendering</summary>
/// <param name ="stride"> </param>
/// <param name ="stride">  </param>
/// <param name ="identifier"> the name of the buffer</param>
/// <remark> calls the iasetvertexbuffer, iasetindexbuffer, iasetprimtivetoplogy(trianglelist) </remark>
void D3D11::SetupBuffer(unsigned int stride, unsigned int offset, std::string identifier)
{
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	m_deviceContext->IASetVertexBuffers(0, 1, &m_buffers[identifier]->t_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	m_deviceContext->IASetIndexBuffer(m_buffers[identifier]->t_indiceBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
}

/// <summary> create a buffer with the given parameters</summary>
/// <param name ="vertices"> the vertices as an array </param>
/// <param name ="indices"> the indices as an array </param>
/// <param name ="verticesCount"> the amount of vertices </param>
/// <param name ="indicesCount"> the amount of indices </param>
/// <param name ="identifier"> the name of the buffer </param>
/// <returns> succes yes/no?</returns>
/// <remark> only call this to create a model buffer</remark>
bool D3D11::CreateModel(CustomModelType* vertices, int verticesCount, unsigned long* indices, int indicesCount, std::string identifier)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	
	// create a struct that will hold the vertices and indices
	BufferStruct* buffer = new BufferStruct();
	
	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(CustomModelType) * verticesCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;



	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//create a buffer for the vertices
	m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &buffer->t_vertexBuffer);

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indicesCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	
	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// create a buffer for the indices
	m_device->CreateBuffer(&indexBufferDesc, &indexData, &buffer->t_indiceBuffer);
	
	// Add the buffer to the map with the identifier as key
	m_buffers[identifier] = buffer;

	return true;
}

/// <summary> creates a device and deviceContext</summary>
/// <returns> succes yes/no?</returns>
/// <remark> only has to be called once to make a device and deviceContext </remark>
bool D3D11::CreateDevice()
{
	HRESULT result;
	int error;
	unsigned int stringLength;
	DXGI_ADAPTER_DESC adapterDesc;
	D3D_FEATURE_LEVEL featureLevel;

	// Set the feature level to DirectX 11.
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create a DirectX graphics interface factory.
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&m_factory);
	if(FAILED(result))
	{
		return false;
	}

	// Use the factory to create an adapter for the primary graphics interface (video card).
	result = m_factory->EnumAdapters(0, &m_adapter);
	if(FAILED(result))
	{
		return false;
	}

	// Enumerate the primary adapter output (monitor).
	result = m_adapter->EnumOutputs(0, &m_adapterOutput);
	if(FAILED(result))
	{
		return false;
	}
	
	// Get the adapter (video card) description.
	result = m_adapter->GetDesc(&adapterDesc);
	if(FAILED(result))
	{
		return false;
	}

	// Store the dedicated video card memory in megabytes.
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
	if(error != 0)
	{
		return false;
	}

	//create the Direct3D device, and Direct3D device context.
	result = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1, 
										   D3D11_SDK_VERSION, &m_device, NULL, &m_deviceContext);
	if(FAILED(result))
	{
		//loggermanager
		return false;
	}

	return true;
}

bool D3D11::CreateSkydomeShader(std::string vsFilename, std::string psFilename, std::string shaderName)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC gradientBufferDesc;

	ShaderStruct* shader = new ShaderStruct();
	// Initialize the pointers this function will use to null.
	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

	std::wstring vstemp = std::wstring(vsFilename.begin(), vsFilename.end());
	const WCHAR* vsFile = vstemp.c_str();

	std::wstring pstemp = std::wstring(psFilename.begin(), psFilename.end());
	const WCHAR* psFile = pstemp.c_str();

	// Compile the vertex shader code.
	result = D3DX11CompileFromFile(vsFile, NULL, NULL, "TVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
				       &vertexShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			//loggermanager
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			//loggermanager
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFile, NULL, NULL, "TPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
				       &pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			//loggermanager
		}
		// If there was nothing in the error message then it simply could not find the file itself.
		else
		{
			//loggermanager
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &shader->t_vertexShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &shader->t_pixelShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the vertex input layout description.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = m_device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), 
										 &shader->t_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(CustomMatrixStruct);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = m_device->CreateBuffer(&matrixBufferDesc, NULL, &shader->t_matrixBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Setup the description of the gradient constant buffer that is in the pixel shader.
	gradientBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	gradientBufferDesc.ByteWidth = sizeof(CustomGradientBufferType);
	gradientBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	gradientBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	gradientBufferDesc.MiscFlags = 0;
	gradientBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = m_device->CreateBuffer(&gradientBufferDesc, NULL, &shader->t_gradientBuffer);
	if(FAILED(result))
	{
		return false;
	}

	m_shaders[shaderName] = shader;

	return true;
}

bool D3D11::SetupSkydomeShader(CustomMatrix view, CustomMatrix world, CustomMatrix projection, CustomGradientBufferType gradientColor,std::string shaderName)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	CustomMatrixStruct* dataPtr;
	CustomGradientBufferType* dataPtr2;
	unsigned int bufferNumber;

	D3DXMATRIX viewMatrix;
	D3DXMATRIX worldMatrix;
	D3DXMATRIX projectionMatrix;

	// create d3dxmatrix from the custom matrix so directx knows what to do with them
	CreateD3DXMatrix(viewMatrix, view);
	CreateD3DXMatrix(worldMatrix, world);
	CreateD3DXMatrix(projectionMatrix, projection);

	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = m_deviceContext->Map(m_shaders[shaderName]->t_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (CustomMatrixStruct*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->t_world = worldMatrix;
	dataPtr->t_view = viewMatrix;
	dataPtr->t_projection = projectionMatrix;

	// Unlock the constant buffer.
	m_deviceContext->Unmap(m_shaders[shaderName]->t_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finally set the constant buffer in the vertex shader with the updated values.
	m_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_shaders[shaderName]->t_matrixBuffer);

	// Lock the gradient constant buffer so it can be written to.
	result = m_deviceContext->Map(m_shaders[shaderName]->t_gradientBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (CustomGradientBufferType*)mappedResource.pData;

	// Copy the gradient color variables into the constant buffer.
	dataPtr2->apexColor = gradientColor.apexColor;
	dataPtr2->centerColor = gradientColor.centerColor;

	// Unlock the constant buffer.
	m_deviceContext->Unmap(m_shaders[shaderName]->t_gradientBuffer, 0);

	// Set the position of the gradient constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the gradient constant buffer in the pixel shader with the updated values.
	m_deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_shaders[shaderName]->t_gradientBuffer);

	return true;
}

/// <summary> create a shader using HLSL files</summary>
/// <param name ="vsFilename"> the vsfilename (location) </param>
/// <param name ="psFilename"> the psfilename (location) </param>
/// <returns> succes yes/no? </return>
bool D3D11::CreateShader(std::string vsFilename, std::string psFilename , std::string shaderName)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightbufferDesc;

	ShaderStruct* shader = new ShaderStruct();
	errorMessage = NULL;
	vertexShaderBuffer = NULL;
	pixelShaderBuffer = NULL;

	//convert the std string to a WCHAR* so we can use it in the d3dx11compilefromfilefunction
	std::wstring vstemp = std::wstring(vsFilename.begin(), vsFilename.end());
	const WCHAR* vsFile = vstemp.c_str();

	std::wstring pstemp = std::wstring(psFilename.begin(), psFilename.end());
	const WCHAR* psFile = pstemp.c_str();

	// Compile the vertex shader code.
	result =  D3DX11CompileFromFile(vsFile, NULL, NULL, "TVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
					   &vertexShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			//loggermanager do your thing
		}
		// If there was nothing in the error message then it simply could not find the shader file itself.
		else
		{
			//loggermanager do your thing
		}

		return false;
	}

	// Compile the pixel shader code.
	result = D3DX11CompileFromFile(psFile, NULL, NULL, "TPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, 
					   &pixelShaderBuffer, &errorMessage, NULL);
	if(FAILED(result))
	{
		// If the shader failed to compile it should have writen something to the error message.
		if(errorMessage)
		{
			//loggermanager do your thing
		}
		// If there was  nothing in the error message then it simply could not find the file itself.
		else
		{
			//loggermanager do your thing
		}

		return false;
	}

	// Create the vertex shader from the buffer.
	result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &shader->t_vertexShader);
	if(FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &shader->t_pixelShader);
	if(FAILED(result))
	{
		return false;
	}

	// Now setup the layout of the data that goes into the shader.
	// This setup needs to match the CustomVertexType stucture in the ModelClass and in the shader.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "COLOR";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = m_device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), 
					   vertexShaderBuffer->GetBufferSize(), &shader->t_layout);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	// Setup the description for the samplerdesc
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;


	// create a samplerstate using the samplerdesc description
	result = m_device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if(FAILED(result))
	{
		return false;
	}


	// Setup the description of the dynamic matrix
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(CustomMatrixStruct);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = m_device->CreateBuffer(&matrixBufferDesc, NULL, &shader->t_matrixBuffer);
	if(FAILED(result))
	{
		return false;
	}
	
	// Setup the description for the lighting
	lightbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightbufferDesc.ByteWidth = sizeof(CustomLightBufferType);
	lightbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightbufferDesc.MiscFlags = 0;
	lightbufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can acces the vertex shader constant buffer from within this class
	result = m_device->CreateBuffer(&lightbufferDesc, NULL, &shader->t_lighting);
	if(FAILED(result))
	{
		return false;
	}

	m_shaders[shaderName] = shader;

	return true;
}

/// <summary>`sets up the data ready for the vertex and pixel shader</summary>
/// <param name ="textureName"> the texture(name) that needs to have a vertex and pixel shader</param>
/// <param name ="view"> the view matrix </param>
/// <param name ="world">the world matrix </param>
/// <param name ="projection"> the projection matrix </param>
/// <param name ="lightbuffer"> the lightbuffer struct that holds the light values</param>
/// <returns>succes yes/no? </returns>
bool D3D11::SetupShader(std::string textureName,  CustomMatrix view, CustomMatrix world, CustomMatrix projection, CustomLightBufferType lightbuffer, std::string shaderName)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	CustomMatrixStruct* dataPtr;
	CustomLightBufferType* dataPtr2;
	unsigned int bufferNumber;

	D3DXMATRIX viewMatrix;
	D3DXMATRIX worldMatrix;
	D3DXMATRIX projectionMatrix;

	// create d3dxmatrix from the custom matrix so directx knows what to do with them
	CreateD3DXMatrix(viewMatrix,view);
	CreateD3DXMatrix(worldMatrix, world);
	CreateD3DXMatrix(projectionMatrix, projection);
	
	// Transpose the matrices to prepare them for the shader.
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = m_deviceContext->Map(m_shaders[shaderName]->t_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = static_cast<CustomMatrixStruct*>(mappedResource.pData);

	// Copy the matrices into the constant buffer.
	dataPtr->t_world = worldMatrix;
	dataPtr->t_view = viewMatrix;
	dataPtr->t_projection = projectionMatrix;

	// Unlock the constant buffer.
	m_deviceContext->Unmap(m_shaders[shaderName]->t_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finaly set the constant buffer in the vertex shader with the updated values.
	m_deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_shaders[shaderName]->t_matrixBuffer);

	result = m_deviceContext->Map(m_shaders[shaderName]->t_lighting, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}
	// Get a pointer to the date in the constant buffer
	dataPtr2 = static_cast<CustomLightBufferType*>(mappedResource.pData);
	// copy the light variable into the constant buffer
	dataPtr2->ambientColor = lightbuffer.ambientColor;
	dataPtr2->diffuseColor = lightbuffer.diffuseColor;
	dataPtr2->lightDirection = lightbuffer.lightDirection;
	dataPtr2->padding = 0.0f;

	//unlock the constant buffer
	m_deviceContext->Unmap(m_shaders[shaderName]->t_lighting, 0);

	//set the position of the constant buffer in the vertex shader
	bufferNumber = 0;

	// set the constant buffer in the vertex shader with the update values 
	m_deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_shaders[shaderName]->t_lighting);
	
	// set the shader resources
	m_deviceContext->PSSetShaderResources(0, 1, &m_texture[textureName]);
	
	return true;
}

/// <summary> creates a d3dxmatrix</summary>
/// <param name ="d3dXMatrix"> the output matrix </param>
/// <param name ="customMatrix"> the input matrix that needs to be converted to a d3dxmatrix </param>
/// <remark> custommatrix does not support math functions yet, so convert it to a d3dxmatrix before calculations </remark>
void D3D11::CreateD3DXMatrix(D3DXMATRIX& d3dXMatrix, CustomMatrix customMatrix)
{
	d3dXMatrix = D3DXMATRIX(customMatrix.t_11, customMatrix.t_12, customMatrix.t_13, customMatrix.t_14,
							customMatrix.t_21, customMatrix.t_22, customMatrix.t_23, customMatrix.t_24,
							customMatrix.t_31, customMatrix.t_32, customMatrix.t_33, customMatrix.t_34,
							customMatrix.t_41, customMatrix.t_42, customMatrix.t_43, customMatrix.t_44);
}

/// <summary>call this every frame</summary>
/// <param name ="indexCount"> the amount of indices </param>
/// <remark> sets the shaders ready for rendering</remark>
void D3D11::Frame(int indexCount, std::string shaderName)
{
	// Set the vertex input layout.
	m_deviceContext->IASetInputLayout(m_shaders[shaderName]->t_layout);

	// Set the vertex and pixel shaders that will be used to render
	m_deviceContext->VSSetShader(m_shaders[shaderName]->t_vertexShader, NULL, 0);
	m_deviceContext->PSSetShader(m_shaders[shaderName]->t_pixelShader, NULL, 0);
	//set the sampler in the pixelshader
	m_deviceContext->PSSetSamplers(0, 1, &m_sampleState);
	// Render
	m_deviceContext->DrawIndexed(indexCount, 0, 0);
}

/// <summary>translate the world matrix everytime the function is called</summary>
/// <param name ="position"> the position the world matrix needs to be translated to </param>
/// <param name ="identifier">the window that needs to have its world translated </param>
void D3D11::TranslateMatrix(CustomVector3 position, std::string identifier, CustomMatrix& customMatrix)
{
	D3DXMATRIX d3dxmatrix;
	D3DXMatrixTranslation(&d3dxmatrix, position.t_x, position.t_y, position.t_z);
	customMatrix = CustomMatrix(d3dxmatrix._11, d3dxmatrix._12, d3dxmatrix._13, d3dxmatrix._14,
								d3dxmatrix._21, d3dxmatrix._22, d3dxmatrix._23, d3dxmatrix._24,
								d3dxmatrix._31, d3dxmatrix._32, d3dxmatrix._33, d3dxmatrix._34,
								d3dxmatrix._41, d3dxmatrix._42, d3dxmatrix._43, d3dxmatrix._44);

}

/// <summary> creates a texture</summary>
/// <param name ="textureName"> the texturename (location) where a texture needs to be created from </param>
/// <returns>succes yes/no? </returns>
/// <remark> add the texture to the texture map</remark>
bool D3D11::CreateTexture(std::string textureName)
{
	HRESULT result;
	ID3D11ShaderResourceView* texture;
	//conver the std string to a WCHAR* for the d3dx11 function.
	std::wstring stemp = std::wstring(textureName.begin(), textureName.end());
	const WCHAR* textureNameAsWCHAR = stemp.c_str();

	result = D3DX11CreateShaderResourceViewFromFile(m_device, textureNameAsWCHAR, NULL, NULL, &texture, NULL);
	if(FAILED(result))
	{
		//loggermanager
		return false;
	}
	//add the texture to the texture map.
	m_texture[textureName] = texture;
	return true;
}

/// <summary>creates a rotation from the world</summary>
/// <param name ="identifier"> the window that needs to rotate its world matrix </param>
/// <param name ="rotation"> the angle of the rotation </param>
void D3D11::RotationMatrix(float rotation, std::string identifier, CustomMatrix& customMatrix)
{
	D3DXMATRIX d3dxmatrix;
	D3DXMatrixRotationY(&d3dxmatrix, rotation);
	customMatrix = CustomMatrix(d3dxmatrix._11, d3dxmatrix._12, d3dxmatrix._13, d3dxmatrix._14,
								d3dxmatrix._21, d3dxmatrix._22, d3dxmatrix._23, d3dxmatrix._24,
								d3dxmatrix._31, d3dxmatrix._32, d3dxmatrix._33, d3dxmatrix._34,
								d3dxmatrix._41, d3dxmatrix._42, d3dxmatrix._43, d3dxmatrix._44);
}

/// <summary>create a vertex and indice buffer for the terrain</summary>
/// <param name ="identifier">the name of the terrain that needs a vertex and indice buffer </param>
/// <param name ="vertices"> the vertices as an array </param>
/// <param name ="verticesCount"> the amount of vertices </param>
/// <param name ="indices"> the indices as an array </param>
/// <param name ="indicesCount"> the amount of indices </param>
/// <returns> succes yes/no? </returns>
/// <remark> only call this to create a terrain</remark>
bool D3D11::CreateTerrain(CustomVertexType* vertices, int verticesCount, unsigned long* indices, int indicesCount, std::string identifier)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indiceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indiceData;
	HRESULT result;
	BufferStruct* terrainBuffer = new BufferStruct();

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(CustomVertexType) * verticesCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &terrainBuffer->t_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indiceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indiceBufferDesc.ByteWidth = sizeof(unsigned long) * indicesCount;
	indiceBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indiceBufferDesc.CPUAccessFlags = 0;
	indiceBufferDesc.MiscFlags = 0;
	indiceBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indiceData.pSysMem = indices;
	indiceData.SysMemPitch = 0;
	indiceData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = m_device->CreateBuffer(&indiceBufferDesc, &indiceData, &terrainBuffer->t_indiceBuffer);
	if(FAILED(result))
	{
		return false;
	}

	m_buffers[identifier] = terrainBuffer;

	return true;
}

/// <summary>create a vertex and indice buffer for the skydome</summary>
/// <param name ="vertices"> the vertices as an array </param>
/// <param name ="verticesCount"> the amount of vertices </param>
/// <param name ="indices"> the indices as an array </param>
/// <param name ="indicesCount"> the amount of indices </param>
/// <param name ="identifier"> the skydome that needs to create a vertex and indice buffer </param>
/// <returns> succes yes/no?</returns>
/// <remark>only call this to create a buffer for the skydome </remark>
bool D3D11::CreateSkydome(CustomVector3* vertices, int verticesCount, unsigned long* indices, int indicesCount, std::string identifier)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;

	BufferStruct* skydomeStruct = new BufferStruct();

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(CustomVector3) * verticesCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &skydomeStruct->t_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indicesCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &skydomeStruct->t_indiceBuffer);
	if(FAILED(result))
	{
		return false;
	}

	m_buffers[identifier] = skydomeStruct;
	
	return true;
}

/// <summary> turn on the culling by setting it back on cullstate back</summary>
void D3D11::TurnOnCulling()
{
	m_deviceContext->RSSetState(m_rasterState);
}

/// <summary> turn off the culling bu setting it to cullstate none</summary>
void D3D11::TurnOffCulling()
{
	m_deviceContext->RSSetState(m_rasterStateNoCulling);
}

/// <summary> turn on the zbuffer by setting the depthstencstate.depthenable on true</summary>
void D3D11::TurnOnZBuffer()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
}

/// <summary> turn off the zbuffer by setting the depthstencstate.depthenable on false</summary>
void D3D11::TurnOffZBuffer()
{
	m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1);
}