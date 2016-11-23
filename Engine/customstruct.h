#ifndef _CUSTOMSTRUCT_H_
#define _CUSTOMSTRUCT_H_

class CustomMatrix
{
public:
	CustomMatrix(){};
	CustomMatrix(float _11, float _12, float _13, float _14,
			     float _21, float _22, float _23, float _24,
		         float _31, float _32, float _33, float _34,
		         float _41, float _42, float _43, float _44)
	{ 
		t_11 = _11, t_12 = _12, t_13 = _13, t_14 = _14,
		t_21 = _21, t_22 = _22, t_23 = _23, t_24 = _24,
		t_31 = _31, t_32 = _32, t_33 = _33, t_34 = _34,
		t_41 = _41, t_42 = _42, t_43 = _43, t_44 = _44;
	};

	float t_11; float t_12; float t_13; float t_14;
	float t_21; float t_22; float t_23; float t_24;
    float t_31; float t_32; float t_33; float t_34;
	float t_41; float t_42; float t_43; float t_44;
	
};

class CustomVector2
{
public:
	CustomVector2(){};
	CustomVector2(float _x, float _y)
	{
		t_x = _x, t_y = _y;
	};

	float t_x; float t_y;
};


class CustomVector3
{
public:
	CustomVector3(){};
	CustomVector3(float _x, float _y, float _z)
	{
		t_x = _x, t_y = _y, t_z = _z;
	};

	float t_x; float t_y; float t_z;
};

class CustomVector4
{
public:
	CustomVector4(){};
	CustomVector4(float _x, float _y, float _z, float _v)
	{
		t_x = _x, t_y = _y, t_z = _z, t_v = _v;
	};

	float t_x; float t_y; float t_z; float t_v;
};


struct CustomVertexType
{
	CustomVector3 position;
	CustomVector2 texture;
	CustomVector3 normal;
	CustomVector4 color;
	//CustomVector4 padding;
};

struct CustomModelType
{
	CustomVector3 position;
	CustomVector2 texture;
	CustomVector3 normal;
};

struct CustomColorType
{
	CustomVector3 position;
	CustomVector4 color;
	float padding;
};

struct CustomGradientBufferType
{
	CustomVector4 apexColor;
	CustomVector4 centerColor;
};

struct CustomLightBufferType
{
	CustomVector4 ambientColor;
	CustomVector4 diffuseColor;
	CustomVector3 lightDirection;
	float padding; // to make it 16 byte align
};
#endif