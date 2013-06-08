//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// RenderItem 2D square in 3D space for rendering game objects 
// 
// Add a summary of your changes here:
// 
// - Fixed vertex rendering by correcting render order and a missing negative on vertex 1
// - Added texture rendering with alpha blending
// - Moved matrix calculations to this method

#include "RenderItem.h"
#include <string>

using Engine::RenderItem;

RenderItem::RenderItem(LPDIRECT3DDEVICE9 p_dx_device) : 
	m_pDxDevice(p_dx_device), p_dx_VertexBuffer(0), p_dx_IndexBuffer(0), flt_Angle(0)
{
	D3DXMatrixIdentity( &m_World );
	FillVertices();
	FillIndices();
	m_Texture = 0;
}

RenderItem::~RenderItem(void)
{
	if( m_Texture != 0 )
	{
		CleanUp();
	}
	p_dx_VertexBuffer->Release();
	p_dx_IndexBuffer->Release();
}

void RenderItem::CleanUp() 
{
	if( m_Texture != 0 )
	{
		m_Texture->Release();
	}
}

void RenderItem::Init(const std::wstring textureFile)
{
	VOID* p_Indices;
	VOID* p_Vertices;
	const size_t vertexSize = 4 * sizeof(VertexPosTex);
	const size_t indexSize = 6 * sizeof(short);

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
	
	UseTexture(textureFile);
}

void RenderItem::UseTexture( const std::wstring textureFile )
{
	// In the case of a texture change, clean up the previous texture
	CleanUp();
	if (FAILED(D3DXCreateTextureFromFile(m_pDxDevice, textureFile.c_str(), &m_Texture)))
	{
		throw std::runtime_error("Error trying to open texture file");
	} 

	m_pDxDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDxDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDxDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	//Set the alpha to come completely from the diffuse component
	m_pDxDevice->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_SELECTARG2);
	m_pDxDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_DIFFUSE);
	m_pDxDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_TEXTURE);
}

void RenderItem::Draw() const
{
	Draw(&m_World);
}

void RenderItem::Draw(D3DXVECTOR3 & translation) const
{
	Draw( translation, D3DXVECTOR3(1,1,1) );
}

void RenderItem::Draw(D3DXVECTOR3 & translation, D3DXVECTOR3 & scale) const
{
	Draw( translation, scale, 0 );
}

void RenderItem::Draw( D3DXVECTOR3 & translation, D3DXVECTOR3 & scale, float zRotationAngle ) const
{
	D3DXMATRIX zRot;
	D3DXMATRIX world;
	D3DXMATRIX move;
	D3DXMATRIX scaling;

	D3DXMatrixRotationZ(&zRot, zRotationAngle);
	D3DXMatrixTranslation( &move, translation.x, translation.y, translation.z );
	D3DXMatrixScaling( &scaling, scale.x, scale.y, scale.z );
		
	world = zRot * scaling * move;
	Draw( &world );
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
	const float half_size = 0.5f;
	// Top right
	cv_Vertices[0].x = half_size;
	cv_Vertices[0].y = half_size;
	cv_Vertices[0].z = 0.0f;
	cv_Vertices[0].colour = 0xFFFFFFFF;
	cv_Vertices[0].tu = 1.0f;
	cv_Vertices[0].tv = 0.0f;

	// Top left
	cv_Vertices[1].x = -half_size;
	cv_Vertices[1].y = half_size;
	cv_Vertices[1].z = 0.0f;
	cv_Vertices[0].colour = 0xFFFFFFFF;
	cv_Vertices[1].tu = 0.0f;
	cv_Vertices[1].tv = 0.0f;

	// Bottom left
	cv_Vertices[2].x = -half_size;
	cv_Vertices[2].y = -half_size;
	cv_Vertices[2].z = 0.0f;
	cv_Vertices[0].colour = 0xFFFFFFFF;
	cv_Vertices[2].tu = 0.0f;
	cv_Vertices[2].tv = 1.0f;

	// Bottom right
	cv_Vertices[3].x = half_size;
	cv_Vertices[3].y = -half_size;
	cv_Vertices[3].z = 0.0f;
	cv_Vertices[0].colour = 0xFFFFFFFF;
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
