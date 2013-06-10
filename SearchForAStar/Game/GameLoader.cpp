#include "GameLoader.h"
#include "../../Core/JSONParser.h"

using namespace SFAS::Game;
using namespace Engine::JSON;

GameLoader::GameLoader()
{

}

GameProperties * GameLoader::LoadGamePropertiesFromFile( const std::string file ) const
{
	JSONParser parser;
	GameProperties * properties = 0;

	// If 0 the parse has failed
	const JSONMapNode * rootNode = parser.ParseJSONFile( file );

	if( rootNode != NULL )
	{
		properties = new GameProperties;
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

void GameLoader::ParseGameFile( const JSONMapNode * root, GameProperties * properties ) const
{
	properties->enemyTypeCount;
	properties->weaponTypesCount;
	
	JSONMapNode * enemyTypes  = (JSONMapNode*) (*root)["enemyTypes"];
	JSONMapNode * weaponTypes = (JSONMapNode*) (*root)["weaponTypes"];

	ParseEnemyTypes( enemyTypes , properties );
	ParseWeaponTypes( weaponTypes, properties );
}

void GameLoader::ParseEnemyTypes( const JSONMapNode * node, GameProperties * properties ) const
{

}

void GameLoader::ParseWeaponTypes( const JSONMapNode * node, GameProperties * properties ) const
{

}