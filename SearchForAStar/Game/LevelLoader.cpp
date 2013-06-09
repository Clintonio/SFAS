#include "LevelLoader.h"
#include "Level.h"
#include "../../Core/JSONParser.h"

using namespace SFAS::Game;

LevelLoader::LevelLoader()
{

}

const Level* LevelLoader::LoadLevelFromFile( std::string file )
{
	Engine::JSONParser parser;
	Level* level = new Level;

	parser.ParseJSONFile( file );

	std::string error;
	if( parser.HasError() )
	{
		error = parser.GetErrorMessage();
	}


	return level;
}