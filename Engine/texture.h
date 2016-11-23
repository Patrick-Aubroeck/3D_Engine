#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//standard library includes
#include <string> 

//own includes
#include "i_renderer.h"

class Texture
{
public:
	Texture(); // standard constructor
	~Texture(); //standard destructor
	bool Initialize(I_Renderer* renderer, std::string filepath); //initialize a texture by using the filepath of the texture and a renderer who holds it 
	void Shutdown();
};

#endif