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
	RenderItem(LPDIRECT3DDEVICE9 p_dx_Device);
	virtual ~RenderItem(void);

	// Initialise vertices, indicies and texture
	void Init( const std::wstring textureFile );
	// Load a new texture. Does not affect verticies and indicies
	void UseTexture( const std::wstring textureFile );
	void Draw() const;
	void Draw( D3DXVECTOR3 & translation ) const;
	void Draw( D3DXVECTOR3 & translation, D3DXVECTOR3 & scale ) const;
	void Draw( D3DXVECTOR3 & translation, D3DXVECTOR3 & scale, float zRotation ) const;
	void Draw( const D3DMATRIX * pWorld ) const;

protected:
	D3DXMATRIX m_World;

private:
	// In the case of texture swapping we want to erase old texture data
	void CleanUp();
	void FillVertices();
	void FillIndices();

	VertexPosTex cv_Vertices[4];
	short s_Indices[6];
	LPDIRECT3DDEVICE9 m_pDxDevice;
	LPDIRECT3DVERTEXBUFFER9 p_dx_VertexBuffer;
	LPDIRECT3DINDEXBUFFER9 p_dx_IndexBuffer;
	float flt_Angle;
	IDirect3DTexture9 * m_Texture;
};
}