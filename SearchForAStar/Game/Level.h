// Level description format

#pragma once

#include <string>
#include <d3dx9.h>

namespace SFAS
{
namespace Game
{

struct Level
{
	struct Enemy 
	{
		//unsigned int type;
		D3DXVECTOR3 pos;
		
		// struct SpawnCondition

		Enemy(
			D3DXVECTOR3 pos
		) : pos(pos) { };
	};
	struct Prop
	{
		D3DXVECTOR3 pos;
	};

	std::string id;
	std::string name;
	//std::string musicFile;
	std::wstring skyboxTextureFile;
	unsigned int enemyCount;
	Level::Enemy* enemies;

	Level(
		std::string id, 
		std::string name, 
		unsigned int enemyCount, 
		Level::Enemy* enemies,
		std::wstring skyboxTexture
	) : 
		id(id), 
		name(name), 
		enemyCount(enemyCount),
		enemies(enemies),
		skyboxTextureFile(skyboxTexture) { }
	
};
}
}