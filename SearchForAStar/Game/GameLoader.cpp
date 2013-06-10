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
		std::string error = "Could not find game properties file";
		if( parser.HasError() )
		{
			error = parser.GetErrorMessage();
		}
		throw new std::runtime_error( error );
	}

	return properties;
}

void GameLoader::ParseGameFile( const JSONMapNode * root, GameProperties * properties ) const
{
	JSONArrayNode * weaponTypes = (JSONArrayNode*) (*root)["weaponTypes"];
	JSONArrayNode * enemyTypes  = (JSONArrayNode*) (*root)["enemyTypes"];
	JSONArrayNode * gameModes   = (JSONArrayNode*) (*root)["gameModes"];

	root->CheckForNodeErrors( weaponTypes, "weaponTypes", JSONType::Array ); 
	root->CheckForNodeErrors( enemyTypes, "enemyTypes", JSONType::Array ); 
	root->CheckForNodeErrors( gameModes, "gameModes", JSONType::Array ); 
	
	properties->enemyTypeCount		= enemyTypes->childCount;
	properties->weaponTypesCount	= weaponTypes->childCount;
	properties->gameModeCount		= gameModes->childCount;

	properties->enemyTypes	= new EnemyType[properties->enemyTypeCount];
	properties->weaponTypes	= new WeaponType[properties->weaponTypesCount];
	properties->gameModes	= new GameMode[properties->gameModeCount];
	
	ParseWeaponTypes( weaponTypes, properties );
	ParseEnemyTypes( enemyTypes , properties );
	ParseGameModes( gameModes , properties );
}

void GameLoader::ParseEnemyTypes( const JSONArrayNode * node, GameProperties * properties ) const
{
	for( unsigned int i = 0; i < properties->enemyTypeCount; i++ )
	{
		JSONMapNode * enemyNode = (JSONMapNode*) node->child[i];
		enemyNode->CheckForNodeErrors( enemyNode, "enemyTypes", JSONType::Object ); 

		properties->enemyTypes[i].id			= enemyNode->GetChildInt( "id" );
		properties->enemyTypes[i].health		= enemyNode->GetChildInt( "health" );
		properties->enemyTypes[i].textureFile	= enemyNode->GetChildWString( "textureFile" );
		properties->enemyTypes[i].dimensions	= enemyNode->GetChildVector3( "dimensions" );
		properties->enemyTypes[i].speed			= enemyNode->GetChildFloat( "speed" );
		properties->enemyTypes[i].killScore		= enemyNode->GetChildInt( "killScore" );
		properties->enemyTypes[i].weaponType	= &properties->weaponTypes[enemyNode->GetChildInt( "weaponType" )];
		properties->enemyTypes[i].aiType		= enemyNode->GetChildString( "AIType" );
	}
}

void GameLoader::ParseWeaponTypes( const JSONArrayNode * node, GameProperties * properties ) const
{
	for( unsigned int i = 0; i < properties->weaponTypesCount; i++ )
	{
		JSONMapNode * weaponNode = (JSONMapNode*) node->child[i];
		weaponNode->CheckForNodeErrors( weaponNode, "weaponTypes", JSONType::Object ); 
		
		properties->weaponTypes[i].name			= weaponNode->GetChildString( "name" );
		properties->weaponTypes[i].damage		= weaponNode->GetChildInt( "damage" );
		properties->weaponTypes[i].ranged		= weaponNode->GetChildBool( "ranged" );
		properties->weaponTypes[i].fireDelay	= weaponNode->GetChildFloat( "fireDelay" );
		properties->weaponTypes[i].lifetime		= weaponNode->GetChildFloat( "lifetime" );
		properties->weaponTypes[i].dimensions	= weaponNode->GetChildVector3( "dimensions" );
		properties->weaponTypes[i].soundFile	= weaponNode->GetChildString( "soundFile" );
		properties->weaponTypes[i].speed		= weaponNode->GetChildFloat( "speed" );
		properties->weaponTypes[i].textureFile	= weaponNode->GetChildWString( "textureFile" );
		properties->weaponTypes[i].weaponAIType	= weaponNode->GetChildString( "AIType" );
	}
}

void GameLoader::ParseGameModes( const JSONArrayNode * node, GameProperties * properties ) const
{
	for( unsigned int i = 0; i < properties->gameModeCount; i++ )
	{
		JSONMapNode * modeNode		= (JSONMapNode*) node->child[i];
		JSONArrayNode * levelNode	= (JSONArrayNode*) (*modeNode)["levels"];
		modeNode->CheckForNodeErrors( modeNode, "gameModes", JSONType::Object ); 
		modeNode->CheckForNodeErrors( levelNode, "levels", JSONType::Array ); 
		
		properties->gameModes[i].name	= modeNode->GetChildString( "name" );
		properties->gameModes[i].type	= modeNode->GetChildString( "type" );
		properties->gameModes[i].lives	= modeNode->GetChildInt( "lives" );
		properties->gameModes[i].numLevels	= levelNode->childCount;

		properties->gameModes[i].levels = new std::string[levelNode->childCount];
		for( unsigned int j = 0; j < levelNode->childCount; j++ )
		{
			properties->gameModes[i].levels[j]	= levelNode->GetChildString( j );
		}
	}
}