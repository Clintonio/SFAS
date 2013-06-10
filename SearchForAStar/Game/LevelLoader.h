// Loads levels from dedicated level files

#pragma once

#include <string>
#include <d3dx9.h>
#include "../../Core/JSONParser.h"


using namespace Engine::JSON;

namespace SFAS
{
namespace Game
{
struct Level;
struct WeaponType;

class LevelLoader
{
public:
	LevelLoader();

	const Level* LoadLevelFromFile( std::string file );
private:
	char* LoadJSONString( std::string file ) const;
	void ParseLevelFile( const JSONMapNode * root, Level * level ) const;
	void ParseEnemies( const JSONArrayNode * enemyNodes, Level * level ) const;

	WeaponType * m_WeaponType;

	// These are some data extraction methods that provide a cleaner interface
	// to the JSON data
	const std::string GetObjectChildStringValue( const JSONMapNode * node, const std::string nodeName ) const;
	const std::wstring GetObjectChildWStringValue( const JSONMapNode * node, const std::string nodeName ) const;
	const int GetObjectChildIntValue( const JSONMapNode * node, const std::string nodeName ) const;
	const D3DXVECTOR3 GetObjectChildVector3Value( const JSONMapNode * node, const std::string nodeName ) const;
	// Throws exceptions for the data extraction functions
	void CheckForNodeErrors( const JSONNode * node, const std::string nodeName, const JSONType & expectedType ) const;
};
}
}