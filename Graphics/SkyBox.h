// Skybox for background

#pragma once

#include <string>
#include <d3dx9.h>
#include "RenderItem.h"

namespace Engine
{
class RenderItem;
class SkyBox
{
public:
	SkyBox(LPDIRECT3DDEVICE9 dx_Device, int width, int height);

	void Init(std::wstring textureFile);

	void Render();
private:
	int m_Width, m_Height;
	LPDIRECT3DDEVICE9 m_dxDevice;
	RenderItem m_RenderItem;
};
}