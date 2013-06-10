// Level description format

#pragma once

#include <string>
#include <d3dx9.h>
#include "GameProperties.h"

namespace SFAS
{
namespace Game
{

struct Level
{
	struct Player {
		D3DXVECTOR3		startPos;
		WeaponType		weapon;
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
	std::wstring			name;
	std::wstring			introText;
	//std::string musicFile;
	unsigned int		enemyCount;
	Level::Enemy*		enemies;
	std::wstring		skyboxTextureFile;
	Level::Player		player;
};
}
}