//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// The camera class pretty basic position and target affair.
// 
// Add a summary of your changes here:
// - Disabled 3D lighting due to lack of lighting source
// - Added initialisation function to handle several of the render states
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

void Camera::Init( LPDIRECT3DDEVICE9 p_dx_Device )
{
	p_dx_Device->SetRenderState(D3DRS_LIGHTING, false);
	p_dx_Device->SetRenderState(D3DRS_ZENABLE, true );
	p_dx_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// Setting up blending and transparency
	p_dx_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	p_dx_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	p_dx_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	/*p_dx_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	p_dx_Device->SetRenderState(D3DRS_ALPHAREF, (DWORD)50);
	p_dx_Device->SetRenderState(D3DRS_ALPHATESTENABLE, 1);
	p_dx_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);*/
}

void Camera::Update( LPDIRECT3DDEVICE9 p_dx_Device )
{
	D3DXMatrixLookAtLH(&m_View, &m_EyePos, &m_TargetPos, &m_UpVector);
	p_dx_Device->SetTransform(D3DTS_VIEW, &m_View);

	D3DXMatrixOrthoLH(&m_Projection, m_Width, m_Height, 1, 200);
	p_dx_Device->SetTransform(D3DTS_PROJECTION, &m_Projection);
}


