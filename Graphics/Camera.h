//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The camera class pretty basic position and target affair.
// 
// Add a summary of your changes here:
// - Added initialisation function
// 
// 
#pragma once

namespace Engine
{

class Camera
{
public:
	Camera( float w, float h );
	virtual ~Camera(void);

	void Camera::Init( LPDIRECT3DDEVICE9 p_dx_Device );
	void Update( LPDIRECT3DDEVICE9 p_dx_Device ); 

private:

	D3DXVECTOR3 m_EyePos;
	D3DXVECTOR3 m_TargetPos;
	D3DXVECTOR3 m_UpVector;
	D3DXMATRIXA16 m_View;
	D3DXMATRIX m_Projection;
	float m_Width;
	float m_Height;

};

}