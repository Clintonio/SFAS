// Level description format

#pragma once

#include <string>
#include <d3dx9.h>
#include "WeaponType.h"

namespace SFAS
{
namespace Game
{

struct Level
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

	struct Enemy 
	{
		unsigned int	type;
		D3DXVECTOR3		pos;

		Enemy(
			unsigned int type,
			D3DXVECTOR3 pos
		) : type(type), pos(pos) { };
	};

	struct Prop
	{
		D3DXVECTOR3 pos;
	};

	std::string			id;
	std::string			name;
	//std::string musicFile;
	unsigned int		enemyTypeCount;
	EnemyType*			enemyTypes;
	unsigned int		enemyCount;
	Level::Enemy*		enemies;
	std::wstring		skyboxTextureFile;
	Level::WeaponType*	weaponTypes;
};
}
}