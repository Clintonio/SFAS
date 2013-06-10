// Loads levels from dedicated level files

#pragma once

#include <string>
#include <d3dx9.h>
#include "../../Core/JSONParser.h"


using namespace Engine::JSON;

namespace SFAS
{
namespace Game
{
struct Level;
struct WeaponType;

class LevelLoader
{
public:
	LevelLoader();

	const Level* LoadLevelFromFile( std::string file );
private:
	char* LoadJSONString( std::string file ) const;
	void ParseLevelFile( const JSONMapNode * root, Level * level ) const;
	void ParseEnemies( const JSONArrayNode * enemyNodes, Level * level ) const;

	WeaponType * m_WeaponType;
};
}
}