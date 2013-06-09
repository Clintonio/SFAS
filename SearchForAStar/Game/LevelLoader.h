// Loads levels from dedicated level files

#pragma once

#include <string>

namespace SFAS
{
namespace Game
{
struct Level;

class LevelLoader
{
public:
	LevelLoader();

	const Level* LoadLevelFromFile( std::string file );
private:
	char* LoadJSONString( std::string file ) const;
};
}
}