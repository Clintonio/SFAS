#include "TextureLoader.h"
#include "RenderItem.h"

using namespace Engine;

TextureLoader::TextureLoader(LPDIRECT3DDEVICE9 p_dx_device) 
	: m_pDxDevice(p_dx_device)
{

}

TextureLoader::~TextureLoader()
{
	std::map<const std::wstring, RenderItem*>::iterator it;
	for(it = m_RenderItems.begin(); it != m_RenderItems.end(); it++)
	{
		RenderItem* tmp = (RenderItem*) (*it).second;
		it++;
		delete tmp;
	}
}

// Load a pooled textured render item
RenderItem* TextureLoader::LoadTexturedRenderItem(const std::wstring texture, float size)
{
	RenderItem* item;
	// Find an existing texture or generate and insert a new one
	std::map<const std::wstring, RenderItem*>::iterator foundItem = m_RenderItems.find(texture);
	if(foundItem != m_RenderItems.end()) 
	{
		item = (RenderItem*) foundItem->second;
	}
	else
	{
		item = new RenderItem(m_pDxDevice);
		item->Init(texture);
		m_RenderItems.insert(std::pair<const std::wstring, RenderItem*>(texture, item));
	}
	return item;
}
