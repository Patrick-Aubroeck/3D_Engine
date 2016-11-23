#include "texture.h"

///<summary>standard constructor </summary>
Texture::Texture() 
{

}

///<summary>standard destructor </summary>
Texture::~Texture()
{

}

/// <summary> initialize the texture by calling the create texture function in the renderer</summary>
/// <param name ="renderer"> the renderer that calls the createtexture function </param>
/// <param name ="filepath"> the filepath of the texture </param>
/// <returns> succes yes/no? </returns>
/// <seealso cref="Renderer.CreateTexture()"/>
bool Texture::Initialize(I_Renderer* renderer, std::string filepath)
{
	HRESULT result;

	result = renderer->CreateTexture(filepath);
	if(!result)
	{
		return false;
	}
	return true;
}

/// <summary> shutdown the texture and calls its member variables shutdown function</summary>
void Texture::Shutdown()
{
	// nothing to do for now
}