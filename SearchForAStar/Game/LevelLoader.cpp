#include "LevelLoader.h"
#include "Level.h"
#include "../../Core/JSONParser.h"

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
}

//===
// Danger Ahead
//===

inline const std::string LevelLoader::GetObjectChildStringValue( const Engine::JSON::JSONMapNode * node, const std::string nodeName ) const
{
	return *((std::string*) ((*node)[nodeName])->value);
}

inline const std::wstring LevelLoader::GetObjectChildWStringValue( const Engine::JSON::JSONMapNode * node, const std::string nodeName ) const
{
	std::string str = *((std::string*) ((*node)[nodeName])->value);

	return std::wstring( str.begin(), str.end() );
}

inline const int LevelLoader::GetObjectChildIntValue( const Engine::JSON::JSONMapNode * node, const std::string nodeName ) const
{
	return *((int*) ((*node)[nodeName])->value);
}

const D3DXVECTOR3 LevelLoader::GetObjectChildVector3Value( const Engine::JSON::JSONMapNode * node, const std::string nodeName ) const
{
	JSONArrayNode * arrayNode = (JSONArrayNode*) (*node)[nodeName];
	float x = *((float*) ((*arrayNode).child[0])->value);
	float y = *((float*) ((*arrayNode).child[1])->value);
	float z = *((float*) ((*arrayNode).child[2])->value);

	return D3DXVECTOR3( x, y, z );
}