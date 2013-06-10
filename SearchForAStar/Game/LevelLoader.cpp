#include "LevelLoader.h"
#include "Level.h"

using namespace SFAS::Game;

using namespace Engine::JSON;

LevelLoader::LevelLoader()
{

}

const Level* LevelLoader::LoadLevelFromFile( std::string file )
{
	JSONParser parser;
	Level* level = new Level;
	ZeroMemory( level, sizeof( Level ) );

	// If 0 the parse has failed
	const JSONMapNode * rootNode = parser.ParseJSONFile( file );

	if( rootNode != NULL )
	{
		ParseLevelFile( rootNode, level );
	}
	else
	{
		std::string error;
		if( parser.HasError() )
		{
			error = parser.GetErrorMessage();
		}
	}

	return level;
}

void LevelLoader::ParseLevelFile( const JSONMapNode * root, Level * level ) const
{
	JSONMapNode * playerNode;
	
	level->id					= root->GetObjectChildStringValue( "id" );
	level->name					= root->GetObjectChildStringValue( "name" );
	level->skyboxTextureFile	= root->GetObjectChildWStringValue( "skyboxTextureFile" );
	level->enemyCount			= root->GetObjectChildIntValue( "enemyCount" );

	playerNode = (JSONMapNode*) (*root)["player"];
	//level->player.weapon		= m_WeaponType[GetObjectChildIntValue( playerNode, "weapon" )];
	level->player.textureFile	= playerNode->GetObjectChildWStringValue( "textureFile" );
	level->player.startPos		= playerNode->GetObjectChildVector3Value( "startPos" );

	ParseEnemies( (JSONArrayNode*) (*root)["enemies"], level );
}

void LevelLoader::ParseEnemies( const Engine::JSON::JSONArrayNode * enemyNodes, Level * level ) const
{
	Level::Enemy * enemies = static_cast<Level::Enemy*> (::operator new (sizeof(Level::Enemy) * level->enemyCount));
	for( unsigned int i = 0; i < level->enemyCount; i++ )
	{
		JSONMapNode * enemyNode = (JSONMapNode*) enemyNodes->child[i];
		enemies[i].type = enemyNode->GetObjectChildIntValue( "type" );
		enemies[i].pos = enemyNode->GetObjectChildVector3Value( "pos" );
	}

	level->enemies = enemies;
}