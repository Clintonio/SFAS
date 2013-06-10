// Stores enemy types, weapon types, etc

#pragma once

#include <string>
#include <d3dx9.h>

namespace SFAS
{
namespace Game
{
// Defines the types of weapons available
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

// Define the types of enemy that exist in the game
struct EnemyType 
{
	unsigned int	id;
	int				health;
	std::string		aiType;
	WeaponType*		weaponType;
	D3DXVECTOR3		dimensions;
	std::wstring	textureFile;
};

// This structure defines the game mode,
// it is primarily for level order specification
struct GameMode
{
	std::string		name;
	std::string		type;
	std::string	*	levels;
	int				lives;
};

// This meta object stores most of the generated
// game properties that can't be gained from level
// loading alone, and level loading usually depends on it
struct GameProperties
{
	unsigned int		enemyTypeCount;
	EnemyType *			enemyTypes;
	unsigned int		weaponTypesCount;
	WeaponType *		weaponTypes;
	unsigned int		gameModeCount;
	GameMode *			gameModes;
};
}
}