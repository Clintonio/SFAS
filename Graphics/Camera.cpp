//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The camera class pretty basic position and target affair.
// 
// Add a summary of your changes here:
// - Disabled 3D lighting due to lack of lighting source
// 
// 

#include <d3d9.h> 
#include <d3dx9.h> 

#include "Camera.h"

using Engine::Camera;

Camera::Camera(float w, float h) : m_EyePos( w * 0.5f, h * 0.5f, -30), m_TargetPos( w * 0.5f, h * 0.5f, 0), m_UpVector(0, 1, 0), m_View(), m_Projection(), m_Width( w ), m_Height( h )
{
}

Camera::~Camera(void)
{
}

void Camera::Update( LPDIRECT3DDEVICE9 p_dx_Device )
{
	D3DXMatrixLookAtLH(&m_View, &m_EyePos, &m_TargetPos, &m_UpVector);
	p_dx_Device->SetTransform(D3DTS_VIEW, &m_View);

	D3DXMatrixOrthoLH(&m_Projection, m_Width, m_Height, 1, 200);
	p_dx_Device->SetTransform(D3DTS_PROJECTION, &m_Projection);

	p_dx_Device->SetRenderState(D3DRS_LIGHTING, false);
}


