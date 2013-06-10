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
	properties->enemyTypeCount		= root->GetChildInt( "enemyTypeCount" );
	properties->weaponTypesCount	= root->GetChildInt( "weaponTypeCount" );
	properties->enemyTypes			= new EnemyType[properties->enemyTypeCount];
	properties->weaponTypes			= new WeaponType[properties->weaponTypesCount];
	
	JSONArrayNode * weaponTypes = (JSONArrayNode*) (*root)["weaponTypes"];
	JSONArrayNode * enemyTypes  = (JSONArrayNode*) (*root)["enemyTypes"];
	root->CheckForNodeErrors( weaponTypes, "enemyTypes", JSONType::Array ); 
	root->CheckForNodeErrors( enemyTypes, "enemyTypes", JSONType::Array ); 
	
	ParseWeaponTypes( weaponTypes, properties );
	ParseEnemyTypes( enemyTypes , properties );
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