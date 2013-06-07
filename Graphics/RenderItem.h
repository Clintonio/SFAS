//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// RenderItem 2D square in 3D space for rendering game objects 
// 
// Add a summary of your changes here:
// - Added texture rendering
// 
// 
#pragma once

#include <d3d9.h> 
#include <d3dx9.h> 
#include <string>

namespace Engine
{

class RenderItem
{
	const static DWORD kFVFState = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	struct VertexPosTex {
		float x, y, z;
		DWORD colour;
		float tu,tv;
	};

public:
	RenderItem(LPDIRECT3DDEVICE9 p_dx_Device, HWND han_Window, float size);
	virtual ~RenderItem(void);

	void Init(const std::wstring textureFile);
	void Draw() const;
	void Draw( const D3DMATRIX * pWorld ) const;

protected:
	D3DXMATRIX m_World;

private:
	void FillVertices();
	void FillIndices();

	VertexPosTex cv_Vertices[4];
	float m_Size;
	short s_Indices[6];
	HWND m_Window;
	LPDIRECT3DDEVICE9 m_pDxDevice;
	LPDIRECT3DVERTEXBUFFER9 p_dx_VertexBuffer;
	LPDIRECT3DINDEXBUFFER9 p_dx_IndexBuffer;
	float flt_Angle;
	IDirect3DTexture9 * m_Texture;
};
}