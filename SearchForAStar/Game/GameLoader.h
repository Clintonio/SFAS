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

	GameProperties LoadGamePropertiesFromFile( std::string file );

private:

	void ParseGameFile( const JSONMapNode * root, GameProperties properties ) const;
};
}
}