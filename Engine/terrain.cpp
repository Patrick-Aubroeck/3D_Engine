#include "terrain.h"
///<summary>standard constructor </summary>
Terrain::Terrain() :
	m_heightMap(NULL),
	m_indiceCount(0),
	m_terrainHeight(0),
	m_terrainWidth(0),
	m_vertexCount(0),
	m_textureRepeat(32)
{

}
/// <summary>standard destructor call the shutdown function instead </summary>
Terrain::~Terrain()
{

}

/// <summary>initialize a terrain with the given terrainfile, texture, color and normalizer</summary>
/// <param name ="renderer">the renderer that will call the createterrain function </param>
/// <param name ="resourceManager">the resourcemanager that needs to load the texture and color file</param>
/// <param name ="terrainFileName"> the file path of the terrainfile</param>
/// <param name ="textureFileName">the file path of the texturefile</param>
/// <param name ="colorFileName">the file path of the color file</param>
/// <param name ="normalizer"> the size the heightmap should be normalized with</param>
/// <returns>returns true if it could create a terrain , otherwise return false </returns>
/// <seealso cref="LoadHeightMap()"/>
/// <seealso cref="NormalizeHeightMap()"/>
/// <seealso cref="CalculateNormals()"/>
/// <seealso cref="CalculateTextureCoordinates()"/>
/// <seealso cref="LoadTexture()"/>
/// <seealso cref="LoadColor()"/>
/// <seealso cref="InitializeBuffers()"/>
/// <seealso cref="renderer.CreateTerrain()"/>
bool Terrain::Initialize(I_Renderer* renderer, ResourceManager* resourceManager, std::string terrainFileName, std::string textureFileName, std::string colorFileName, float normalizer)
{
	bool result; 

	result = LoadHeightMap(terrainFileName);
	if(!result)
	{
		//loggermanager
		return false;
	}
	
	NormalizeHeightMap(normalizer);

	result = CalculateNormals();
	if(!result)
	{
		return false;
	}

	CalculateTextureCoordinates();

	result = LoadTexture(resourceManager, textureFileName);
	if(!result)
	{
		return false;
	}

	result = LoadColorMap(resourceManager,colorFileName);
	if(!result)
	{
		return false;
	}

	result = InitializeBuffers(renderer, terrainFileName);
	if(!result)
	{
		
		return false;
	}
	return true;
}

/// <summary> initialize the vertices and indices , and give them to to renderer in the createterrain function</summary>
/// <param name ="renderer"> the renderer that needs to call the createterrain function</param>
/// <param name ="terrainTexturename"> the path of the terraintexture </param>
/// <returns> succes yes/no?</returns>
/// <seealso cref="renderer.CreateTerrain()"/>
bool Terrain::InitializeBuffers(I_Renderer* renderer, std::string terrainTexturename)
{
	bool result;
	CustomVertexType* terrainVertices;
	unsigned long* terrainIndices;
	int index;
	int index1;
	int index2;
	int index3;
	int index4;
	float tu;
	float tv;
	
	m_vertexCount = (m_terrainWidth -1) * (m_terrainHeight -1) * 6;

	m_indiceCount = m_vertexCount;

	terrainVertices = new CustomVertexType[m_vertexCount];
	if(!terrainVertices)
	{
		return false;
	}
	terrainIndices = new unsigned long[m_indiceCount];
	if(!terrainIndices)
	{
		return false;
	}

	// Initialize the index for the vertex buffer.
	index = 0;

	// Load the vertex and index array with the terrain data.
	for(int j = 0; j<(m_terrainHeight-1); j++)
	{
		for(int i = 0; i<(m_terrainWidth-1); i++)
		{
			index1 = (m_terrainHeight * j) + i;          // Bottom left.
			index2 = (m_terrainHeight * j) + (i+1);      // Bottom right.
			index3 = (m_terrainHeight * (j+1)) + i;      // Upper left.
			index4 = (m_terrainHeight * (j+1)) + (i+1);  // Upper right.

			// Upper left.
			tv = m_heightMap[index3].texture.t_y;

			// Modify the texture coordinates to cover the top edge.
			if(tv == 1.0f) 
			{ 
				tv = 0.0f; 
			}

			terrainVertices[index].position = CustomVector3(m_heightMap[index3].position.t_x, m_heightMap[index3].position.t_y, m_heightMap[index3].position.t_z);
			terrainVertices[index].texture = CustomVector2(m_heightMap[index3].texture.t_x, tv);
			terrainVertices[index].normal = CustomVector3(m_heightMap[index3].normal.t_x, m_heightMap[index3].normal.t_y, m_heightMap[index3].normal.t_z);
			terrainVertices[index].color = CustomVector4(m_heightMap[index3].color.t_x, m_heightMap[index3].color.t_y, m_heightMap[index3].color.t_z, 1.0f);
			terrainIndices[index] = index;
			index++;

			// Upper right.
			tu = m_heightMap[index4].texture.t_x;
			tv = m_heightMap[index4].texture.t_y;

			// Modify the texture coordinates to cover the top and right edge.
			if(tu == 0.0f) { tu = 1.0f; }
			if(tv == 1.0f) { tv = 0.0f; }

			terrainVertices[index].position = CustomVector3(m_heightMap[index4].position.t_x, m_heightMap[index4].position.t_y, m_heightMap[index4].position.t_z);
			terrainVertices[index].texture = CustomVector2(tu, tv);
			terrainVertices[index].normal = CustomVector3(m_heightMap[index4].normal.t_x, m_heightMap[index4].normal.t_y, m_heightMap[index4].normal.t_z);
			terrainVertices[index].color = CustomVector4(m_heightMap[index4].color.t_x, m_heightMap[index4].color.t_y, m_heightMap[index4].color.t_z, 1.0f);
			terrainIndices[index] = index;
			index++;

			// Bottom left.
			terrainVertices[index].position = CustomVector3(m_heightMap[index1].position.t_x, m_heightMap[index1].position.t_y, m_heightMap[index1].position.t_z);
			terrainVertices[index].texture = CustomVector2(m_heightMap[index1].texture.t_x, m_heightMap[index1].texture.t_y);
			terrainVertices[index].normal = CustomVector3(m_heightMap[index1].normal.t_x, m_heightMap[index1].normal.t_y, m_heightMap[index1].normal.t_z);
			terrainVertices[index].color = CustomVector4(m_heightMap[index1].color.t_x, m_heightMap[index1].color.t_y, m_heightMap[index1].color.t_z, 1.0f);
			terrainIndices[index] = index;
			index++;

			// Bottom left.
			terrainVertices[index].position = CustomVector3(m_heightMap[index1].position.t_x, m_heightMap[index1].position.t_y, m_heightMap[index1].position.t_z);
			terrainVertices[index].texture = CustomVector2(m_heightMap[index1].texture.t_x, m_heightMap[index1].texture.t_y);
			terrainVertices[index].normal = CustomVector3(m_heightMap[index1].normal.t_x, m_heightMap[index1].normal.t_y, m_heightMap[index1].normal.t_z);
			terrainVertices[index].color = CustomVector4(m_heightMap[index1].color.t_x, m_heightMap[index1].color.t_y, m_heightMap[index1].color.t_z, 1.0f);
			terrainIndices[index] = index;
			index++;

			// Upper right.
			tu = m_heightMap[index4].texture.t_x;
			tv = m_heightMap[index4].texture.t_y;

			// Modify the texture coordinates to cover the top and right edge.
			if(tu == 0.0f) { tu = 1.0f; }
			if(tv == 1.0f) { tv = 0.0f; }

			terrainVertices[index].position = CustomVector3(m_heightMap[index4].position.t_x, m_heightMap[index4].position.t_y, m_heightMap[index4].position.t_z);
			terrainVertices[index].texture = CustomVector2(tu, tv);
			terrainVertices[index].normal = CustomVector3(m_heightMap[index4].normal.t_x, m_heightMap[index4].normal.t_y, m_heightMap[index4].normal.t_z);
			terrainVertices[index].color = CustomVector4(m_heightMap[index4].color.t_x, m_heightMap[index4].color.t_y, m_heightMap[index4].color.t_z, 1.0f);
			terrainIndices[index] = index;
			index++;

			// Bottom right.
			tu = m_heightMap[index2].texture.t_x;

			// Modify the texture coordinates to cover the right edge.
			if(tu == 0.0f) { tu = 1.0f; }

			terrainVertices[index].position = CustomVector3(m_heightMap[index2].position.t_x, m_heightMap[index2].position.t_y, m_heightMap[index2].position.t_z);
			terrainVertices[index].texture = CustomVector2(tu, m_heightMap[index2].texture.t_y);
			terrainVertices[index].normal = CustomVector3(m_heightMap[index2].normal.t_x, m_heightMap[index2].normal.t_y, m_heightMap[index2].normal.t_z);
			terrainVertices[index].color = CustomVector4(m_heightMap[index2].color.t_x, m_heightMap[index2].color.t_y, m_heightMap[index2].color.t_z, 1.0f);
			terrainIndices[index] = index;
			index++;
		}
	}

	result = renderer->CreateTerrain(terrainVertices,m_vertexCount,terrainIndices,m_indiceCount,"../Engine/data/dirt01.dds");
	if(!result)
	{
		return false;
	}

	// Release the arrays now that the buffers have been created and loaded.
	delete [] terrainVertices;
	terrainVertices = NULL;

	delete [] terrainIndices;
	terrainIndices = NULL;

	return true;
}

/// <summary> shutdown the member variables </summary>
/// <seealso cref="ShutdownHeightMap()"/>
void Terrain::Shutdown()
{
	ShutdownHeightMap();
}

/// <summary>render the buffers that has been initialized</summary>
/// <param name ="renderer"> the renderer that has to call the setupbuffer function </param>
/// <param name ="terrainTexturename"> the path of the terraintexture </param>
/// <seealso cref="RenderBuffers()"/>
void Terrain::Render(I_Renderer* renderer, std::string terrainTexturename)
{
	RenderBuffers(renderer, terrainTexturename);
}

/// <summary>render the buffers that has been initialized with a pre set stride and offset</summary>
/// <param name ="renderer"> the renderer that has to call the setupbuffer function </param>
/// <param name ="terrainTexturename"> the path of the terraintexture</param>
/// <seealso cref="renderer.SetupBuffer()"/>
void Terrain::RenderBuffers(I_Renderer* renderer,std::string terrainTexturename)
{
	unsigned int stride;
	unsigned int offset;
	
	stride = sizeof(CustomVertexType);
	offset = 0;
	renderer->SetupBuffer(stride, offset, terrainTexturename);
}

/// <summary>get the amount of indices</summary>
/// <returns> returns the amount of indices as an integer</returns>
int Terrain::GetIndexCount()
{
	return m_indiceCount;
}

/// <summary>load in the heightmap using a bitfile</summary>
/// <param name ="terrainFileName"> the path of the terrainfile</param>
/// <returns> succes yes/no?</returns>
/// <remark> the file has to be of a bmp extension </remark>
bool Terrain::LoadHeightMap(std::string terrainFileName)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize;
	int index;

	unsigned char* bitmapImage;
	unsigned char height;
	
	const char* fileName = terrainFileName.c_str();

	error = fopen_s(&filePtr, fileName, "rb");

	if(error != 0)
	{
		return false;
	}

	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	if(count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if(count != 1)
	{
		return false;
	}
	m_terrainWidth = bitmapInfoHeader.biWidth;
	m_terrainHeight = bitmapInfoHeader.biHeight;

	imageSize = m_terrainWidth * m_terrainHeight * 3;

	bitmapImage = new unsigned char[imageSize];
	if(!bitmapImage)
	{
		return false;
	}

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	count = fread(bitmapImage, 1, imageSize, filePtr);
	if(count != imageSize)
	{
		return false;
	}

	error = fclose(filePtr);

	if(error != 0)
	{
		return false;
	}

	m_heightMap = new CustomVertexType[m_terrainWidth * m_terrainHeight];
	if(!m_heightMap)
	{
		return false;
	}

	int k = 0;
	// Read the image data into the height map.
	for(int j = 0; j<m_terrainHeight; j++)
	{
		for(int i = 0; i<m_terrainWidth; i++)
		{
			height = bitmapImage[k];
			
			index = (m_terrainHeight * j) + i;

			m_heightMap[index].position.t_x = (float)i;
			m_heightMap[index].position.t_y = (float)height;
			m_heightMap[index].position.t_z = (float)j;

			k+=3;
		}
	}

	// Release the bitmap image data.
	delete [] bitmapImage;
	bitmapImage = NULL;

	return true;
}

/// <summary>normalize the heightmap</summary>
/// <remark> normalize the heightmap if it looks unrealistic </remark>
void Terrain::NormalizeHeightMap(float normalizer)
{
	for(int j = 0; j<m_terrainHeight; j++)
	{
		for(int i = 0; i<m_terrainWidth; i++)
		{
			m_heightMap[(m_terrainHeight * j) + i].position.t_y /= normalizer;
		}
	}

	return;
}
/// <summary>delete the terrain heightmap that holds the position, texture and normals</summary>
void Terrain::ShutdownHeightMap()
{
	if(m_heightMap)
	{
		delete [] m_heightMap;
		m_heightMap = NULL;
	}

}
/// <summary>calculate the normal values by using phongs algorithm</summary>
/// <returns> succes yes/no? </returns>
bool Terrain::CalculateNormals()
{
	int index;
	int index1;
	int index2;
	int index3;
	int count;
	float vertex1[3];
	float vertex2[3];
	float vertex3[3];
	float vector1[3];
	float vector2[3];
	float sum[3];
	float length;
	CustomVector3* normals;


	// Create a temporary array to hold the un-normalized normal vectors.
	normals = new CustomVector3[(m_terrainHeight-1) * (m_terrainWidth-1)];
	if(!normals)
	{
		return false;
	}
	
	// Go through all the faces in the mesh and calculate their normals.
	for(int j = 0; j<(m_terrainHeight-1); ++j)
	{
		for(int i = 0; i<(m_terrainWidth-1); ++i)
		{
			index1 = (j * m_terrainHeight) + i;
			index2 = (j * m_terrainHeight) + (i+1);
			index3 = ((j+1) * m_terrainHeight) + i;

			// Get three vertices from the face.
			vertex1[0] = m_heightMap[index1].position.t_x;
			vertex1[1] = m_heightMap[index1].position.t_y;
			vertex1[2] = m_heightMap[index1].position.t_z;
		
			vertex2[0] = m_heightMap[index2].position.t_x;
			vertex2[1] = m_heightMap[index2].position.t_y;
			vertex2[2] = m_heightMap[index2].position.t_z;
		
			vertex3[0] = m_heightMap[index3].position.t_x;
			vertex3[1] = m_heightMap[index3].position.t_y;
			vertex3[2] = m_heightMap[index3].position.t_z;

			// Calculate the two vectors for this face.
			vector1[0] = vertex1[0] - vertex3[0];
			vector1[1] = vertex1[1] - vertex3[1];
			vector1[2] = vertex1[2] - vertex3[2];
			vector2[0] = vertex3[0] - vertex2[0];
			vector2[1] = vertex3[1] - vertex2[1];
			vector2[2] = vertex3[2] - vertex2[2];

			index = (j * (m_terrainHeight-1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].t_x = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
			normals[index].t_y = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
			normals[index].t_z = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
		}
	}

	// Now go through all the vertices and take an average of each face normal 	
	// that the vertex touches to get the averaged normal for that vertex.
	for(int j = 0; j<m_terrainHeight; j++)
	{
		for(int i = 0; i<m_terrainWidth; i++)
		{
			// Initialize the sum.
			sum[0] = 0.0f;
			sum[1] = 0.0f;
			sum[2] = 0.0f;

			// Initialize the count.
			count = 0;

			// Bottom left face.
			if(((i-1) >= 0) && ((j-1) >= 0))
			{
				index = ((j-1) * (m_terrainHeight-1)) + (i-1);

				sum[0] += normals[index].t_x;
				sum[1] += normals[index].t_y;
				sum[2] += normals[index].t_z;
				count++;
			}

			// Bottom right face.
			if((i < (m_terrainWidth-1)) && ((j-1) >= 0))
			{
				index = ((j-1) * (m_terrainHeight-1)) + i;

				sum[0] += normals[index].t_x;
				sum[1] += normals[index].t_y;
				sum[2] += normals[index].t_z;
				count++;
			}

			// Upper left face.
			if(((i-1) >= 0) && (j < (m_terrainHeight-1)))
			{
				index = (j * (m_terrainHeight-1)) + (i-1);

				sum[0] += normals[index].t_x;
				sum[1] += normals[index].t_y;
				sum[2] += normals[index].t_z;
				count++;
			}

			// Upper right face.
			if((i < (m_terrainWidth-1)) && (j < (m_terrainHeight-1)))
			{
				index = (j * (m_terrainHeight-1)) + i;

				sum[0] += normals[index].t_x;
				sum[1] += normals[index].t_y;
				sum[2] += normals[index].t_z;
				count++;
			}
			
			// Take the average of the faces touching this vertex.
			sum[0] = (sum[0] / (float)count);
			sum[1] = (sum[1] / (float)count);
			sum[2] = (sum[2] / (float)count);

			// Calculate the length of this normal.
			length = sqrt((sum[0] * sum[0]) + (sum[1] * sum[1]) + (sum[2] * sum[2]));
			
			// Get an index to the vertex location in the height map array.
			index = (j * m_terrainHeight) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			m_heightMap[index].normal.t_x = (sum[0] / length);
			m_heightMap[index].normal.t_y = (sum[1] / length);
			m_heightMap[index].normal.t_z = (sum[2] / length);
		}
	}

	// Release the temporary normals.
	delete [] normals;
	normals = 0;

	return true;
}

/// <summary>load in a texture that will be placed over the terrain</summary>
/// <param name ="resourceManager"> the resourcemanager that has to call the loadtexture function </param>
/// <param name ="textureFileName"> the path of the texturefile </param>
/// <returns> succes yes/no ? </returns>
/// <seealso cref="resourceManager.loadTexture()"/>
bool Terrain::LoadTexture(ResourceManager* resourceManager, std::string textureFileName)
{
	bool result;
	result = resourceManager->LoadTexture(textureFileName);
	if(!result)
	{
		return false;
	}

	return true;
}

/// <summary>calculate the texture coordinates per vertex</summary>
/// <remark>using a texture repeat to smoothen the texture terrain </remark>
void Terrain::CalculateTextureCoordinates()
{
	int incrementCount;
	int tuCount;
	int tvCount;
	float incrementValue;
	float tuCoordinate;
	float tvCoordinate;
	
	// Calculate how much to increment the texture coordinates by.
	incrementValue = (float)m_textureRepeat / (float)m_terrainWidth;

	// Calculate how many times to repeat the texture.
	incrementCount = m_terrainWidth / m_textureRepeat;

	// Initialize the tu and tv coordinate values.
	tuCoordinate = 0.0f;
	tvCoordinate = 1.0f;

	// Initialize the tu and tv coordinate indexes.
	tuCount = 0;
	tvCount = 0;

	// Loop through the entire height map and calculate the tu and tv texture coordinates for each vertex.
	for(int j = 0; j<m_terrainHeight; j++)
	{
		for(int i = 0; i<m_terrainWidth; i++)
		{
			// Store the texture coordinate in the height map.
			m_heightMap[(m_terrainHeight * j) + i].texture.t_x = tuCoordinate;
			m_heightMap[(m_terrainHeight * j) + i].texture.t_y = tvCoordinate;

			// Increment the tu texture coordinate by the increment value and increment the index by one.
			tuCoordinate += incrementValue;
			tuCount++;

			// Check if at the far right end of the texture and if so then start at the beginning again.
			if(tuCount == incrementCount)
			{
				tuCoordinate = 0.0f;
				tuCount = 0;
			}
		}

		// Increment the tv texture coordinate by the increment value and increment the index by one.
		tvCoordinate -= incrementValue;
		tvCount++;

		// Check if at the top of the texture and if so then start at the bottom again.
		if(tvCount == incrementCount)
		{
			tvCoordinate = 1.0f;
			tvCount = 0;
		}
	}
}

/// <summary>load the colorfile and place it over the terrain</summary>
/// <param name ="resourcemanager">the resourcemanager that needs to load the color </param>
/// <param name ="colorFileName">the path of the colorfile </param>
/// <returns>succes yes/no ? </returns>
/// <remark> the colorfile has to be of a bmp extension </remark>
bool Terrain::LoadColorMap(ResourceManager* resourcemanager, std::string colorFileName)
{
	int error;
	int imageSize;
	int index;
	int colorMapWidth;
	int colorMapHeight;
	FILE* filePtr;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	unsigned char* bitmapImage;

	const char* fileName = colorFileName.c_str();
	// Open the color map file in binary.
	error = fopen_s(&filePtr, fileName, "rb");
	if(error != 0)
	{
		return false;
	}

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// Make sure the color map dimensions are the same as the terrain dimensions for easy 1 to 1 mapping.
	colorMapWidth = bitmapInfoHeader.biWidth;
	colorMapHeight = bitmapInfoHeader.biHeight;
	//remove check if you want the color to be able to stretch etc.
	if((colorMapWidth != m_terrainWidth) || (colorMapHeight != m_terrainHeight))
	{
		return false;
	}

	// Calculate the size of the bitmap image data.
	imageSize = colorMapWidth * colorMapHeight * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	if(!bitmapImage)
	{
		return false;
	}

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	if(count != imageSize)
	{
		return false;
	}

	// Close the file.
	error = fclose(filePtr);
	if(error != 0)
	{
		return false;
	}
	// Initialize the position in the image data buffer.
	int k = 0;

	// Read the image data into the color map portion of the height map structure.
	for(int j = 0; j<colorMapHeight; j++)
	{
		for(int i = 0; i<colorMapWidth; i++)
		{
			index = (colorMapHeight * j) + i;

			m_heightMap[index].color.t_z = (float)bitmapImage[k]   / 255.0f;
			m_heightMap[index].color.t_y = (float)bitmapImage[k+1] / 255.0f;
			m_heightMap[index].color.t_x = (float)bitmapImage[k+2] / 255.0f;

			k+=3;
		}
	}

	// Release the bitmap image data.
	delete [] bitmapImage;
	bitmapImage = 0;

	return true;
}