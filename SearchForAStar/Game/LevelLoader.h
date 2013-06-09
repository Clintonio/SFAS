// Loads levels from dedicated level files

#pragma once

#include <string>
#include <d3dx9.h>

namespace Engine
{
namespace JSON
{
	struct JSONMapNode;
}
}

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
	void ParseLevelFile( const Engine::JSON::JSONMapNode * root, Level * level ) const;

	WeaponType * m_WeaponType;

	// These are some ugly convenience methods that provide a nicer interface
	// to the JSON data
	inline const std::string GetObjectChildStringValue( const Engine::JSON::JSONMapNode * node, const std::string nodeName ) const;
	inline const std::wstring GetObjectChildWStringValue( const Engine::JSON::JSONMapNode * node, const std::string nodeName ) const;
	inline const int GetObjectChildIntValue( const Engine::JSON::JSONMapNode * node, const std::string nodeName ) const;
	const D3DXVECTOR3 GetObjectChildVector3Value( const Engine::JSON::JSONMapNode * node, const std::string nodeName ) const;
};
}
}