#include "LevelLoader.h"
#include "Level.h"

using namespace SFAS::Game;

using namespace Engine::JSON;

LevelLoader::LevelLoader( const WeaponType * weaponTypes ) : 
	m_WeaponTypes( weaponTypes )
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
	
	level->id					= root->GetChildString( "id" );
	level->name					= root->GetChildString( "name" );
	level->skyboxTextureFile	= root->GetChildWString( "skyboxTextureFile" );
	level->enemyCount			= root->GetChildInt( "enemyCount" );

	playerNode = (JSONMapNode*) (*root)["player"];
	level->player.weapon		= m_WeaponTypes[playerNode->GetChildInt( "weapon" )];
	level->player.textureFile	= playerNode->GetChildWString( "textureFile" );
	level->player.startPos		= playerNode->GetChildVector3( "startPos" );

	ParseEnemies( (JSONArrayNode*) (*root)["enemies"], level );
}

void LevelLoader::ParseEnemies( const Engine::JSON::JSONArrayNode * enemyNodes, Level * level ) const
{
	Level::Enemy * enemies = static_cast<Level::Enemy*> (::operator new (sizeof(Level::Enemy) * level->enemyCount));
	for( unsigned int i = 0; i < level->enemyCount; i++ )
	{
		JSONMapNode * enemyNode = (JSONMapNode*) enemyNodes->child[i];
		enemies[i].type = enemyNode->GetChildInt( "type" );
		enemies[i].pos = enemyNode->GetChildVector3( "pos" );
	}

	level->enemies = enemies;
}