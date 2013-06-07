#include "SkyBox.h"
#include "RenderItem.h"

using Engine::SkyBox;

SkyBox::SkyBox(LPDIRECT3DDEVICE9 dx_Device, int width, int height) : m_dxDevice(dx_Device), m_Width(width), m_Height(height), m_RenderItem(dx_Device)
{
}

void SkyBox::Init(std::wstring textureFile)
{
	m_RenderItem.Init(textureFile);
}

void SkyBox::Render()
{
	D3DXMATRIX world;
	D3DXMATRIX move;
	D3DXMATRIX scale;

	D3DXMatrixTranslation( &move, (float) m_Width / 2, (float) m_Height / 2, 0 );
	D3DXMatrixScaling( &scale, m_Width, m_Height, 0.0f );
		
	world = scale * move;
	m_RenderItem.Draw( &world );
}