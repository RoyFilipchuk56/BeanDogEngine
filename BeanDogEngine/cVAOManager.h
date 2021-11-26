#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

// Will load the models and place them 
// into the vertex and index buffers to be drawn

#include <string>
#include <map>

// The vertex structure 
//	that's ON THE GPU (eventually) 
// So dictated from THE SHADER
//struct sVertex
//{
//	float x, y, z;		
//	float r, g, b;
//};

struct sVertex_XYZW_RGBA_N_UV_T_B
{
	float x, y, z, w;   // Same as vec4 vPosition
	float r, g, b, a;   // Same as vec4 vColour
	float nx, ny, nz, nw;   // in vec4 vNormal;	Vertex normal X,Y,Z (W ignored)
	float u0, v0, u1, v1;   //in vec4 vUVx2;					// 2 x Texture coords (vec4) UV0, UV1
	float tx, ty, tz, tw;   //in vec4 vTangent;				// For bump mapping X,Y,Z (W ignored)
	float bx, by, bz, bw;   //in vec4 vBiNormal;				// For bump mapping X,Y,Z (W ignored)
};

struct sModelDrawInfo
{
	sModelDrawInfo(); 

	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	// The "local" (i.e. "CPU side" temporary array)
	sVertex_XYZW_RGBA_N_UV_T_B* pVertices;	//  = 0;
//	sVertex_XYZW_RGBA* pVertices;	//  = 0;
//	sVertex_XYZ_RGB* pVertices;	//  = 0;
	// The index buffer (CPU side)
	unsigned int* pIndices;
};


class cVAOManager
{
public:

	bool LoadModelIntoVAO(std::string fileName, 
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID);

	// We don't want to return an int, likely
	bool FindDrawInfoByModelName(std::string filename,
								 sModelDrawInfo &drawInfo);

	// This is modified from GenerateSphericalTextureCoords.cpp file,
	//	specifically that it's using the sModelDrawInfo structure
	enum enumTEXCOORDBIAS
	{
		POSITIVE_X, POSITIVE_Y, POSITIVE_Z
	};

	void GenerateSphericalTextureCoords(
		enumTEXCOORDBIAS uBias, enumTEXCOORDBIAS vBias,
		sModelDrawInfo& drawInfo, bool basedOnNormals, float scale, bool fast);

	// Defaults to POSITIVE_X, POSITIVE_Y
	void GenerateSphericalTextureCoords(
		sModelDrawInfo& drawInfo, bool basedOnNormals, float scale, bool fast);

private:

	std::map< std::string /*model name*/,
		      sModelDrawInfo /* info needed to draw*/ >
		m_map_ModelName_to_VAOID;

};

#endif	// _cVAOManager_HG_
