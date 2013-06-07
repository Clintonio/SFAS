// Textured render item loader and pool

#include <string>
#include <map>
#include <d3d9.h> 

namespace Engine 
{
class RenderItem;

class TextureLoader 
{
public:
	TextureLoader(LPDIRECT3DDEVICE9 p_dx_device, HWND han_Window);
	~TextureLoader();

	// Load a pooled textured render item
	RenderItem* LoadTexturedRenderItem(const std::wstring texture, float size);
private:
	
	HWND m_Window;
	LPDIRECT3DDEVICE9 m_pDxDevice;

	std::map<const std::wstring, RenderItem*> m_RenderItems;
};
}