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

#include <d3d9.h> 
#include <d3dx9.h> 

#include "RenderItem.h"

using Engine::RenderItem;

RenderItem::RenderItem(LPDIRECT3DDEVICE9 p_dx_device, HWND han_Window, float size, DWORD colour) : m_Size(size), m_Window(han_Window), m_pDxDevice(p_dx_device), p_dx_VertexBuffer(0), p_dx_IndexBuffer(0), flt_Angle(0), m_Colour( colour )
{
	D3DXMatrixIdentity( &m_World );
}

RenderItem::~RenderItem(void)
{
}

void RenderItem::Init()
{
	FillVertices();
	FillIndices();

	if (FAILED(m_pDxDevice->CreateVertexBuffer(4*sizeof(VertexPosCol), 0, D3DFVF_XYZ|D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &p_dx_VertexBuffer, NULL)))
	{
		MessageBox(m_Window,L"Error while creating RenderItem VertexBuffer",L"FillVertices()",MB_OK);
	}

	VOID* p_Vertices;
	if (FAILED(p_dx_VertexBuffer->Lock(0, 4*sizeof(VertexPosCol), (void**)&p_Vertices, 0)))
	{
		MessageBox(m_Window,L"Error trying to lock RenderItem VertexBuffer",L"FillVertices()",MB_OK);
	}
	else
	{
		memcpy(p_Vertices, cv_Vertices, 4*sizeof(VertexPosCol));
		p_dx_VertexBuffer->Unlock();
	}

	if (FAILED(m_pDxDevice->CreateIndexBuffer(6*sizeof(short), D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_MANAGED,&p_dx_IndexBuffer,NULL)))
	{
		MessageBox(m_Window,L"Error while creating RenderItem IndexBuffer",L"FillIndices()",MB_OK);
	}

	VOID* p_Indices;
	if (FAILED(p_dx_IndexBuffer->Lock(0, 6*sizeof(short), (void**)&p_Indices, 0)))
	{
		MessageBox(m_Window,L"Error trying to lock RenderItem IndexBuffer",L"FillIndices()",MB_OK);
	}
	else
	{
		memcpy(p_Indices, s_Indices, 6*sizeof(short));
		p_dx_IndexBuffer->Unlock();
	}
}

void RenderItem::Draw()
{
	Draw(&m_World);
}

void RenderItem::Draw( const D3DMATRIX * pWorld )
{
	m_pDxDevice->SetTransform(D3DTS_WORLD, pWorld);
    m_pDxDevice->SetStreamSource(0, p_dx_VertexBuffer, 0, sizeof(VertexPosCol));
    m_pDxDevice->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
	m_pDxDevice->SetIndices(p_dx_IndexBuffer);
	m_pDxDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,0,4,0,2);
}

void RenderItem::FillVertices()
{
	const float half_size = m_Size * 0.5f;
     cv_Vertices[0].x = half_size;
     cv_Vertices[0].y = half_size;
     cv_Vertices[0].z = 0.0f;
     cv_Vertices[0].color = m_Colour;
 
     cv_Vertices[1].x = half_size;
     cv_Vertices[1].y = half_size;
     cv_Vertices[1].z = 0.0f;
     cv_Vertices[1].color = m_Colour;
 
     cv_Vertices[2].x = -half_size;
     cv_Vertices[2].y = -half_size;
     cv_Vertices[2].z = 0.0f;
     cv_Vertices[2].color = m_Colour;

     cv_Vertices[3].x = half_size;
     cv_Vertices[3].y = -half_size;
     cv_Vertices[3].z = 0.0f;
     cv_Vertices[3].color = m_Colour;
}

void RenderItem::FillIndices()
{
     s_Indices[0]=1;
     s_Indices[1]=2;
     s_Indices[2]=0;
     s_Indices[3]=2;
     s_Indices[4]=1;
     s_Indices[5]=3; 
}
