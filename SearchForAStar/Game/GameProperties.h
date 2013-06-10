// Stores enemy types, weapon types, etc

#pragma once

#include "WeaponType.h"
#include <string>
#include <d3dx9.h>

namespace SFAS
{
namespace Game
{
struct EnemyType 
{
	unsigned int	id;
	int				health;
	std::string		aiType;
	WeaponType*		weaponType;
	D3DXVECTOR3		dimensions;
	std::wstring	textureFile;
};

struct GameProperties
{
	unsigned int		enemyTypeCount;
	EnemyType*			enemyTypes;
	unsigned int		weaponTypesCount;
	WeaponType*			weaponTypes;
};
}
}