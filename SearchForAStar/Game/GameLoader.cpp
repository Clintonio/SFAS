#include "GameLoader.h"
#include "../../Core/JSONParser.h"

using namespace SFAS::Game;
using namespace Engine::JSON;

GameLoader::GameLoader()
{

}

GameProperties GameLoader::LoadGamePropertiesFromFile( std::string file )
{
	JSONParser parser;
	GameProperties properties;
	ZeroMemory( &properties, sizeof( GameProperties ) );

	// If 0 the parse has failed
	const JSONMapNode * rootNode = parser.ParseJSONFile( file );

	if( rootNode != NULL )
	{
		ParseGameFile( rootNode, properties );
	}
	else
	{
		std::string error;
		if( parser.HasError() )
		{
			error = parser.GetErrorMessage();
		}
	}

	return properties;
}

void GameLoader::ParseGameFile( const JSONMapNode * root, GameProperties properties ) const
{

}