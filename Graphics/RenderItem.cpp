//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// RenderItem 2D square in 3D space for rendering game objects 
// 
// Add a summary of your changes here:
// 
// - Fixed vertex rendering by correcting render order and a missing negative on vertex 1
// - Added texture rendering

#include "RenderItem.h"
#include <string>

using Engine::RenderItem;

RenderItem::RenderItem(LPDIRECT3DDEVICE9 p_dx_device, HWND han_Window, float size) : 
	m_Size(size), m_Window(han_Window), m_pDxDevice(p_dx_device), p_dx_VertexBuffer(0), p_dx_IndexBuffer(0), flt_Angle(0)
{
	D3DXMatrixIdentity( &m_World );
}

RenderItem::~RenderItem(void)
{
	m_Texture->Release();
}

void RenderItem::Init(const std::wstring textureFile)
{
	VOID* p_Indices;
	VOID* p_Vertices;
	const size_t vertexSize = 4 * sizeof(VertexPosTex);
	const size_t indexSize = 6 * sizeof(short);

	FillVertices();
	FillIndices();

	if (FAILED(m_pDxDevice->CreateVertexBuffer(vertexSize, 0, kFVFState, D3DPOOL_DEFAULT, &p_dx_VertexBuffer, NULL)))
	{
		throw std::runtime_error("Error while creating RenderItem VertexBuffer");
	}
	
	if (FAILED(p_dx_VertexBuffer->Lock(0, vertexSize, (void**)&p_Vertices, 0)))
	{
		throw std::runtime_error("Error trying to lock RenderItem VertexBuffer");
	}

	memcpy(p_Vertices, cv_Vertices, vertexSize);
	p_dx_VertexBuffer->Unlock();

	if (FAILED(m_pDxDevice->CreateIndexBuffer(indexSize, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &p_dx_IndexBuffer, NULL)))
	{
		throw std::runtime_error("Error while creating RenderItem IndexBuffer");
	}

	if (FAILED(p_dx_IndexBuffer->Lock(0, indexSize, (void**)&p_Indices, 0)))
	{
		throw std::runtime_error("Error trying to lock RenderItem IndexBuffer");
	}

	memcpy(p_Indices, s_Indices, indexSize);
	p_dx_IndexBuffer->Unlock();
		
	if (FAILED(D3DXCreateTextureFromFile(m_pDxDevice, textureFile.c_str(), &m_Texture)))
	{
		throw std::runtime_error("Error trying to open texture file");
	} 

	m_pDxDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
	m_pDxDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	m_pDxDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
}

void RenderItem::Draw() const
{
	Draw(&m_World);
}

void RenderItem::Draw( const D3DMATRIX * pWorld ) const
{
	m_pDxDevice->SetTransform(D3DTS_WORLD, pWorld);
    m_pDxDevice->SetStreamSource(0, p_dx_VertexBuffer, 0, sizeof(VertexPosTex));
    m_pDxDevice->SetFVF(kFVFState);
	m_pDxDevice->SetIndices(p_dx_IndexBuffer);
	m_pDxDevice->SetTexture(0, m_Texture);
	m_pDxDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

void RenderItem::FillVertices()
{
	const float half_size = m_Size * 0.5f;
	// Top right
	cv_Vertices[0].x = half_size;
	cv_Vertices[0].y = half_size;
	cv_Vertices[0].z = 0.0f;
	cv_Vertices[0].tu = 1.0f;
	cv_Vertices[0].tv = 0.0f;

	// Top left
	cv_Vertices[1].x = -half_size;
	cv_Vertices[1].y = half_size;
	cv_Vertices[1].z = 0.0f;
	cv_Vertices[1].tu = 0.0f;
	cv_Vertices[1].tv = 0.0f;

	// Bottom left
	cv_Vertices[2].x = -half_size;
	cv_Vertices[2].y = -half_size;
	cv_Vertices[2].z = 0.0f;
	cv_Vertices[2].tu = 0.0f;
	cv_Vertices[2].tv = 1.0f;

	// Bottom right
	cv_Vertices[3].x = half_size;
	cv_Vertices[3].y = -half_size;
	cv_Vertices[3].z = 0.0f;
	cv_Vertices[3].tu = 1.0f;
	cv_Vertices[3].tv = 1.0f;
}

void RenderItem::FillIndices()
{
     s_Indices[0] = 1;
     s_Indices[1] = 0;
     s_Indices[2] = 3;
     s_Indices[3] = 2;
     s_Indices[4] = 1;
     s_Indices[5] = 3; 
}
