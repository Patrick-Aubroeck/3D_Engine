#ifndef _CAMERA_H_
#define _CAMERA_H_

//standard library includes
#include <d3d11.h>
#include <dxgi.h>
#include <math.h> 

//own includes
#include "keyboard.h"
#include "entity.h"
#include "inputmanager.h"
#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\D3DX10math.h"

class Camera : public Entity
{
private:
	CustomVector3 m_position; // holds the position of the camera
	CustomVector3 m_rotation; // holds the rotation of the camera
	CustomMatrix m_viewMatrix; // holds the view matrix of the camera
	CustomVector2 m_moveForwardAndBackwardsSpeed; // holds the forward and backwards accelaration of the camera
	CustomVector2 m_moveUpAndDownwardsSpeed; // holds the up and downward accelaration of the camera
	CustomVector2 m_lookLeftAndRightTurnSpeed; // holds the left and right accelaration of the camera
	CustomVector2 m_lookUpAndLookDownwardsSpeed; // holds the look up and downwards accelaration of the camera
	float m_frameTime; // holds the actual time
	CustomVector3 m_yawPitchRoll; //holds the yaw, pitch and roll of the camera
public:
	Camera(); // constructor
	~Camera(); // destructor 
	void GetViewMatrix(CustomMatrix& view); //returns the viewmatrix
	CustomVector3 GetPosition(); // returns the current position
	CustomVector3 GetRotation();// returns the current rotation
	CustomVector3 GetYawPitchRoll(); //returns the yaw, pitch and roll as a customVector3

	bool Initialize(I_Renderer* renderer, ResourceManager* resourceManager){ return true; };
	

	void Render(I_Renderer* renderer, std::string cameraName); // Setup the camera lookat, yaw, pitch, up and roll factors and create a rotation and view matrix

	void SetPosition(float x, float y, float z); // sets the position of the camera at the given x y z values
	void SetRotation(float x, float y, float z); // sets the rotation of the camera at the given x y z values
	void SetYawPitchRoll(float yaw, float pitch, float roll); //set the yaw, pitch and roll values

	//movement functions
	void SetFrameTime(float time); // sets the actual frame time ( need to call this every frame for smooth camera movement)
	void TurnLeft(bool keydown); // if the param is true turns the camera towards the left at a given speed (if false deaccelarate)
	void TurnRight(bool keydown); // if the param is true turns the camera towards the right at a given speed (if false deaccelarate)
	void LookDown(bool keydown); // if the param is true turns the camera downwards (if false deaccelarate)
	void LookUp(bool keydown); // if the param is true turns the camera upwards (if false deaccelarate)
	void MoveForward(bool keydown); // if the param is true move the camera forwards ( if false deaccelarate)
	void MoveBackward(bool keydown); // if the param is true move the camera backwards ( if false deaccelarate)
	void MoveUpward(bool keydown); // if the param is true move the camera upwards ( if false deaccelarate)
	void MoveDownward(bool keydown); // if the param is true move the camera downwards ( if false deaccelarate)
	void Shutdown(); //shutdown the camera and its member variables
private:
	void SetupCustomMatrix(D3DXMATRIX matrix); // creates a custommatrix from the d3dxmatrix
	void UpdateMouseMovement(); //update the rotation variables with the yaw pitch and roll values
};
#endif