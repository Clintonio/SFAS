//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// RenderItem 2D square in 3D space for rendering game objects 
// 
// Add a summary of your changes here:
// 
// 
// 
#pragma once

#include <d3d9.h>

namespace Engine
{

class RenderItem
{
	struct VertexPosCol
	{
		float x, y, z;
		DWORD color;
	}; 

public:
	RenderItem(LPDIRECT3DDEVICE9 p_dx_Device, HWND han_Window, float size, DWORD colour);
	virtual ~RenderItem(void);

	void Init();
	void Draw();
	void Draw( const D3DMATRIX * pWorld );

protected:
	D3DXMATRIX m_World;

private:
	void FillVertices();
	void FillIndices();

	VertexPosCol cv_Vertices[4];
	float m_Size;
	short s_Indices[6];
	HWND m_Window;
	LPDIRECT3DDEVICE9 m_pDxDevice;
	LPDIRECT3DVERTEXBUFFER9 p_dx_VertexBuffer;
	LPDIRECT3DINDEXBUFFER9 p_dx_IndexBuffer;
	float flt_Angle;
	DWORD m_Colour;
};
}