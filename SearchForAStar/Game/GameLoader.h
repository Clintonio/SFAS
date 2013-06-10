// This class is for loading weapon and enemy types for
// levels to use

#pragma once

#include <string>
#include "GameProperties.h"

namespace Engine
{
namespace JSON
{
struct JSONMapNode;
}
}

using namespace SFAS::Game;
using namespace Engine::JSON;

namespace SFAS
{
namespace Game
{
class GameLoader
{
public:
	GameLoader();

	GameProperties * LoadGamePropertiesFromFile( const std::string file ) const;

private:
	// Loads the game properties from the given JSON map to the properties structure
	void ParseGameFile( const JSONMapNode * root, GameProperties * properties ) const;

	void ParseEnemyTypes( const JSONMapNode * node, GameProperties * properties ) const;
	void ParseWeaponTypes( const JSONMapNode * node, GameProperties * properties ) const;
};
}
}