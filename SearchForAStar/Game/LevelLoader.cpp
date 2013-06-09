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

	if( parser.HasError() )
	{
		unsigned int errorPosition = parser.GetErrorPosition();
	}


	return level;
}