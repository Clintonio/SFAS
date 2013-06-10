// Descriptor for weapon type

#pragma once

#include <string>
#include <d3dx9.h>

namespace SFAS
{
namespace Game
{
struct WeaponType
{
	std::string		name;
	bool			ranged;
	int				damage;
	float			speed;
	float			fireDelay;
	float			lifetime;
	D3DXVECTOR3		dimensions;
	std::string		weaponAIType;
	std::wstring	textureFile;
	std::string		soundFile;
};
}
}