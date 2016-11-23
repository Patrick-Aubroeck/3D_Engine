// own includes
#include "camera.h"

/// <summary> standard constructor set the camera position in the middle of the world </summary>
Camera::Camera() :
	m_position(CustomVector3(128.0f, 17.0f, 128.0f)),
	m_rotation(CustomVector3(0.0f, 0.0f, 0.0f)),
	m_frameTime(0),
	m_lookLeftAndRightTurnSpeed(CustomVector2(0.0f, 0.0f)),
	m_lookUpAndLookDownwardsSpeed(CustomVector2(0.0f, 0.0f)),
	m_moveForwardAndBackwardsSpeed(CustomVector2(0.0f, 0.0f)),
	m_moveUpAndDownwardsSpeed(CustomVector2(0.0f, 0.0f)),
	m_viewMatrix(CustomMatrix(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)),
	m_yawPitchRoll(CustomVector3(0.0f, 0.0f, 0.0f))
{

}

///<summary> standard destructor call the shutdown function first</summary>
Camera::~Camera()
{

}

/// <summary>set the position of the camera using an x y and z coordinate</summary>
/// <param name ="x"> the x position of the camera </param>
/// <param name ="y"> the y position of the camera </param>
/// <param name ="z"> the z position of the camera </param>
void Camera::SetPosition(float x, float y, float z)
{
	m_position = CustomVector3(x, y, z);
}

/// <summary>set the roation of the camera using an x y and z coordinate</summary>
/// <param name ="x"> the x rotation of the camera </param>
/// <param name ="y"> the y rotation of the camera </param>
/// <param name ="z"> the z rotation of the camera </param>
void Camera::SetRotation(float x, float y, float z)
{
	m_rotation= CustomVector3(x, y, z);
}

/// <summary>get the position of the camera that calls this function</summary>
/// <returns> returns the camera position as a customvector3</returns>
CustomVector3 Camera::GetPosition()
{
	return m_position;
}

/// <summary> get the rotation of the camera that calls this function</summary>
/// <returns> returns the camera rotation as a customvector3 </returns>
CustomVector3 Camera::GetRotation()
{
	return m_rotation;
}

/// <summary>get the yaw, pitch and roll of the camera that calls this function</summary>
/// <returns>returns the yaw, pitch and roll of the camera as a customvector3</returns>
CustomVector3 Camera::GetYawPitchRoll()
{
	return m_yawPitchRoll;
}

/// <summary>set the yaw, pitch and roll of the camera that calls this function</summary>

/// <seealso cref="UpdateMouseMovement()" />
void Camera::SetYawPitchRoll(float yaw, float pitch, float roll)
{
	m_yawPitchRoll.t_x = yaw;
	m_yawPitchRoll.t_y = pitch;
	m_yawPitchRoll.t_z = roll;

	UpdateMouseMovement();
}

/// <summary> create a the camera yaw, pitch, roll, up and lookat. also creates a rotation and view matrix </summary>
/// <seealso cref="SetupCustomMatrix()" />
void Camera::Render(I_Renderer* renderer, std::string cameraName)
{
	D3DXVECTOR3 position;
	float radians;
	D3DXMATRIX viewMatrix;
	D3DXMATRIX rotationMatrix;
	D3DXVECTOR3 up;
	D3DXVECTOR3 lookAt;
	up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	position = D3DXVECTOR3(m_position.t_x, m_position.t_y, m_position.t_z);
	lookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	// Calculate the rotation in radians.
	radians = m_rotation.t_y * 0.0174532925f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	m_yawPitchRoll.t_y = m_rotation.t_x * 0.0174532925f;
	m_yawPitchRoll.t_x = m_rotation.t_y * 0.0174532925f;
	m_yawPitchRoll.t_z = m_rotation.t_z * 0.0174532925f;

	//// Setup where the camera is looking.
	//lookAt.x = sinf(radians) + m_position.t_x;
	//lookAt.y = m_position.t_y;
	//lookAt.z = cosf(radians) + m_position.t_z;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_yawPitchRoll.t_x, m_yawPitchRoll.t_y, m_yawPitchRoll.t_z);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Create the view matrix from the three vectors.
	D3DXMatrixLookAtLH(&viewMatrix, &position, &lookAt, &up);

	SetupCustomMatrix(viewMatrix);

}

/// <summary>update the camerarotation by getting the yaw, pitch and roll values</summary>
/// <remark> multiplies the yaw and pitch by *0.1f to make the turning speed look smoother</remark>
void Camera::UpdateMouseMovement()
{
	m_rotation.t_y = m_yawPitchRoll.t_x * 0.1f;
	m_rotation.t_x = m_yawPitchRoll.t_y * 0.1f;
	m_rotation.t_z = m_yawPitchRoll.t_z;
}

/// <summary>set up the viewmatrix by using a d3dxmatrix as input</summary>
/// <param name ="d3dxMatrix"> the d3dxmatrix that is used to create a custommatrix</param>
void Camera::SetupCustomMatrix(D3DXMATRIX d3dxMatrix)
{
	m_viewMatrix = CustomMatrix(d3dxMatrix._11, d3dxMatrix._12, d3dxMatrix._13, d3dxMatrix._14,
								d3dxMatrix._21, d3dxMatrix._22, d3dxMatrix._23, d3dxMatrix._24,
								d3dxMatrix._31, d3dxMatrix._32, d3dxMatrix._33, d3dxMatrix._34,
								d3dxMatrix._41, d3dxMatrix._42, d3dxMatrix._43, d3dxMatrix._44);
}

/// <summary>get the viewmatrix and sets it as the input param</summary>
/// <param name ="viewMatrix"> the matrix that will copy the viewmatrix </param>
void Camera::GetViewMatrix(CustomMatrix& viewMatrix)
{	
	viewMatrix = m_viewMatrix;
}

/// <summary>set the frame time of the camera</summary>
/// <param name ="time"> the time that the frametime will be set to </param>
void Camera::SetFrameTime(float time)
{
	m_frameTime = time;

}
/// <summary>turn the camera to the left if the key was pressed will accelarate. decellarate if it was up</summary>
/// <param name ="keydown"> the boolean that checks if the key is down </param>
void Camera::TurnLeft(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns left.  If not slow down the turn speed.
	if(keydown)
	{
		m_lookLeftAndRightTurnSpeed.t_x += m_frameTime * 0.01f;

		if(m_lookLeftAndRightTurnSpeed.t_x > (m_frameTime * 0.15f))
		{
			m_lookLeftAndRightTurnSpeed.t_x = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookLeftAndRightTurnSpeed.t_x -= m_frameTime* 0.005f;

		if(m_lookLeftAndRightTurnSpeed.t_x < 0.0f)
		{
			m_lookLeftAndRightTurnSpeed.t_x = 0.0f;
		}
	}

	// Update the rotation using the turning speed.
	m_rotation.t_y -= m_lookLeftAndRightTurnSpeed.t_x;
	if(m_rotation.t_y < 0.0f)
	{
		m_rotation.t_y += 360.0f;
	}
}

/// <summary>turn the camera to the right if the key was pressed will accelarate. decellarate if it was up</summary>
/// <param name ="keydown"> the boolean that checks if the key is down </param>
void Camera::TurnRight(bool keydown)
{
	// If the key is pressed increase the speed at which the camera turns right.  If not slow down the turn speed.
	if(keydown)
	{
		m_lookLeftAndRightTurnSpeed.t_y += m_frameTime * 0.01f;

		if(m_lookLeftAndRightTurnSpeed.t_y > (m_frameTime * 0.15f))
		{
			m_lookLeftAndRightTurnSpeed.t_y = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookLeftAndRightTurnSpeed.t_y -= m_frameTime* 0.005f;

		if(m_lookLeftAndRightTurnSpeed.t_y < 0.0f)
		{
			m_lookLeftAndRightTurnSpeed.t_y = 0.0f;
		}
	}
	// Update the rotation using the turning speed.
	m_rotation.t_y += m_lookLeftAndRightTurnSpeed.t_y;
	if(m_rotation.t_y > 360.0f)
	{
		m_rotation.t_y -= 360.0f;
	}
}

/// <summary>turn the camera to look up if the key was pressed will accelarate. decellarate if it was up</summary>
/// <param name ="keydown"> the boolean that checks if the key is down </param>
void Camera::LookUp(bool keydown)
{
	// Update the upward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookUpAndLookDownwardsSpeed.t_x += m_frameTime * 0.01f;

		if(m_lookUpAndLookDownwardsSpeed.t_x > (m_frameTime * 0.15f))
		{
			m_lookUpAndLookDownwardsSpeed.t_x = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookUpAndLookDownwardsSpeed.t_x -= m_frameTime* 0.005f;

		if(m_lookUpAndLookDownwardsSpeed.t_x < 0.0f)
		{
			m_lookUpAndLookDownwardsSpeed.t_x = 0.0f;
		}
	}

	// Update the rotation.
	m_rotation.t_x -= m_lookUpAndLookDownwardsSpeed.t_x;

	// Keep the rotation maximum 90 degrees.
	if(m_rotation.t_x > 90.0f)
	{
		m_rotation.t_x = 90.0f;
	}

	return;
}

/// <summary>turn the camera to look down if the key was pressed will accelarate. decellarate if it was up</summary>
/// <param name ="keydown"> the boolean that checks if the key is down </param>
void Camera::LookDown(bool keydown)
{
	// Update the downward rotation speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_lookUpAndLookDownwardsSpeed.t_y += m_frameTime * 0.01f;

		if(m_lookUpAndLookDownwardsSpeed.t_y > (m_frameTime * 0.15f))
		{
			m_lookUpAndLookDownwardsSpeed.t_y = m_frameTime * 0.15f;
		}
	}
	else
	{
		m_lookUpAndLookDownwardsSpeed.t_y -= m_frameTime* 0.005f;

		if(m_lookUpAndLookDownwardsSpeed.t_y < 0.0f)
		{
			m_lookUpAndLookDownwardsSpeed.t_y = 0.0f;
		}
	}

	// Update the rotation.
	m_rotation.t_x += m_lookUpAndLookDownwardsSpeed.t_y;

	// Keep the rotation maximum 90 degrees.
	if(m_rotation.t_x < -90.0f)
	{
		m_rotation.t_x = -90.0f;
	}

}

/// <summary>move the camera upwards if the key was pressed will accelarate. decellarate if it was up</summary>
/// <param name ="keydown"> the boolean that checks if the key is down </param>
void Camera::MoveUpward(bool keydown)
{
	// Update the upward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_moveUpAndDownwardsSpeed.t_x += m_frameTime * 0.003f;

		if(m_moveUpAndDownwardsSpeed.t_x > (m_frameTime * 0.03f))
		{
			m_moveUpAndDownwardsSpeed.t_x = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_moveUpAndDownwardsSpeed.t_x -= m_frameTime * 0.002f;

		if(m_moveUpAndDownwardsSpeed.t_x < 0.0f)
		{
			m_moveUpAndDownwardsSpeed.t_x = 0.0f;
		}
	}

	// Update the height position.
	m_position.t_y += m_moveUpAndDownwardsSpeed.t_x;
}

/// <summary>move the camera downwards if the key was pressed will accelarate. decellarate if it was up</summary>
/// <param name ="keydown"> the boolean that checks if the key is down </param>
void Camera::MoveDownward(bool keydown)
{
	// Update the downward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_moveUpAndDownwardsSpeed.t_y += m_frameTime * 0.003f;

		if(m_moveUpAndDownwardsSpeed.t_y > (m_frameTime * 0.03f))
		{
			m_moveUpAndDownwardsSpeed.t_y = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_moveUpAndDownwardsSpeed.t_y -= m_frameTime * 0.002f;

		if(m_moveUpAndDownwardsSpeed.t_y < 0.0f)
		{
			m_moveUpAndDownwardsSpeed.t_y = 0.0f;
		}
	}

	// Update the height position.
	m_position.t_y -= m_moveUpAndDownwardsSpeed.t_y;
}

/// <summary>move the camera forward if the key was pressed will accelarate. decellarate if it was up</summary>
/// <param name ="keydown"> the boolean that checks if the key is down </param>
void Camera::MoveForward(bool keydown)
{
	float radians;

	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_moveForwardAndBackwardsSpeed.t_x += m_frameTime * 0.001f;

		if(m_moveForwardAndBackwardsSpeed.t_x > (m_frameTime * 0.03f))
		{
			m_moveForwardAndBackwardsSpeed.t_x = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_moveForwardAndBackwardsSpeed.t_x -= m_frameTime * 0.0007f;

		if(m_moveForwardAndBackwardsSpeed.t_x < 0.0f)
		{
			m_moveForwardAndBackwardsSpeed.t_x = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotation.t_y * 0.0174532925f;

	// Update the position.
	m_position.t_x += sinf(radians) * m_moveForwardAndBackwardsSpeed.t_x;
	m_position.t_z += cosf(radians) * m_moveForwardAndBackwardsSpeed.t_x;
}

/// <summary>move the camera backwards if the key was pressed will accelarate. decellarate if it was up</summary>
/// <param name ="keydown"> the boolean that checks if the key is down </param>
void Camera::MoveBackward(bool keydown)
{
	float radians;


	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		m_moveForwardAndBackwardsSpeed.t_y += m_frameTime * 0.001f;

		if(m_moveForwardAndBackwardsSpeed.t_y > (m_frameTime * 0.03f))
		{
			m_moveForwardAndBackwardsSpeed.t_y = m_frameTime * 0.03f;
		}
	}
	else
	{
		m_moveForwardAndBackwardsSpeed.t_y -= m_frameTime * 0.0007f;
		
		if(m_moveForwardAndBackwardsSpeed.t_y < 0.0f)
		{
			m_moveForwardAndBackwardsSpeed.t_y = 0.0f;
		}
	}

	// Convert degrees to radians.
	radians = m_rotation.t_y * 0.0174532925f;

	// Update the position.
	m_position.t_x -= sinf(radians) * m_moveForwardAndBackwardsSpeed.t_y;
	m_position.t_z -= cosf(radians) * m_moveForwardAndBackwardsSpeed.t_y;
}


/// <summary> shutdown the camera member variables and call its shutdown function</summary>
void Camera::Shutdown()
{
	// nothing to delete
}