// Descriptor for weapon type

#pragma once

#include <string>

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
	std::string		weaponAIType;
	std::wstring	textureFile;
	std::string	soundFile;
};
}
}