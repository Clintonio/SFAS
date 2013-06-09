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
	

	level->id					= GetObjectChildStringValue( root, "id" );
	level->name					= GetObjectChildStringValue( root, "name" );
	level->skyboxTextureFile	= GetObjectChildWStringValue( root, "skyboxTextureFile" );
	level->enemyCount			= GetObjectChildIntValue( root, "enemyCount" );

	playerNode = (JSONMapNode*) (*root)["player"];
	//level->player.weapon		= m_WeaponType[GetObjectChildIntValue( playerNode, "weapon" )];
	level->player.textureFile	= GetObjectChildWStringValue( playerNode, "textureFile" );
	level->player.startPos		= GetObjectChildVector3Value( playerNode, "startPos" );

	ParseEnemies( (JSONArrayNode*) (*root)["enemies"], level );
}

void LevelLoader::ParseEnemies( const Engine::JSON::JSONArrayNode * enemyNodes, Level * level ) const
{
	Level::Enemy * enemies = static_cast<Level::Enemy*> (::operator new (sizeof(Level::Enemy) * level->enemyCount));
	for( unsigned int i = 0; i < level->enemyCount; i++ )
	{
		JSONMapNode * enemyNode = (JSONMapNode*) enemyNodes->child[i];
		enemies[i].type = GetObjectChildIntValue( enemyNode, "type" );
		enemies[i].pos = GetObjectChildVector3Value( enemyNode, "pos" );
	}

	level->enemies = enemies;
}

//===
// Node Converion functions.
//===

const std::string LevelLoader::GetObjectChildStringValue( const Engine::JSON::JSONMapNode * node, const std::string nodeName ) const
{
	const JSONNode * child = (*node)[nodeName];
	CheckForNodeErrors( child, nodeName, JSONType::String );

	return *((std::string*) child->value);
}

const std::wstring LevelLoader::GetObjectChildWStringValue( const Engine::JSON::JSONMapNode * node, const std::string nodeName ) const
{
	const JSONNode * child = (*node)[nodeName];
	CheckForNodeErrors( child, nodeName, JSONType::String );

	std::string str = *((std::string*) child->value);
	return std::wstring( str.begin(), str.end() );
}

const int LevelLoader::GetObjectChildIntValue( const Engine::JSON::JSONMapNode * node, const std::string nodeName ) const
{
	const JSONNode * child = (*node)[nodeName];
	CheckForNodeErrors( child, nodeName, JSONType::Int );
	return *((int*) child->value);
}

const D3DXVECTOR3 LevelLoader::GetObjectChildVector3Value( const Engine::JSON::JSONMapNode * node, const std::string nodeName ) const
{
	JSONArrayNode * arrayNode = (JSONArrayNode*) (*node)[nodeName];
	CheckForNodeErrors( arrayNode, nodeName, JSONType::Array );
	CheckForNodeErrors( arrayNode->child[0], nodeName, JSONType::Float );
	CheckForNodeErrors( arrayNode->child[1], nodeName, JSONType::Float );
	CheckForNodeErrors( arrayNode->child[2], nodeName, JSONType::Float );
	float x = *((float*) (arrayNode->child[0])->value);
	float y = *((float*) (arrayNode->child[1])->value);
	float z = *((float*) (arrayNode->child[2])->value);

	return D3DXVECTOR3( x, y, z );
}

void LevelLoader::CheckForNodeErrors( const Engine::JSON::JSONNode * node, const std::string nodeName, const JSONType & expectedType ) const
{
	if( node == 0 )
	{
		throw LevelLoaderException( nodeName, expectedType);
	}
	
	if ( node->type != expectedType )
	{
		throw LevelLoaderException( nodeName, expectedType, node->type );
	}
}