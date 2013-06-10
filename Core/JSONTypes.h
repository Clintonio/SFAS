// Types for the JSON parser

#pragma once

#include <string>
#include <stdexcept>
#include <d3dx9.h>

namespace Engine
{
namespace JSON
{
class JSONCastException;

enum JSONType
{
	Object,
	Array,
	String,
	Float,
	Int,
	Bool,
	Null
};

// Forward declaration to avoid the circular dependency
struct JSONArrayNode;


struct JSONNode
{
	JSONArrayNode * parent;
	JSONType type;
	void*	value;
};

struct JSONArrayNode : public JSONNode
{
	JSONNode **	child;
	unsigned int	childCount;
	
	const std::string GetChildString( const unsigned int index ) const;

	void CheckForNodeErrors( const Engine::JSON::JSONNode * node, const unsigned int nodeIndex, const JSONType & expectedType ) const;
};

struct JSONMapNode : public JSONArrayNode
{
	std::string	**	childTags; // Only in object

	// Search through child tags for the given tag and return that child
	const JSONNode * operator [] ( const std::string tagName ) const;
	const std::string GetChildString( const std::string nodeName ) const;
	const std::wstring GetChildWString( const std::string nodeName ) const;
	const int GetChildInt( const std::string nodeName ) const;
	const D3DXVECTOR3 GetChildVector3( const std::string nodeName ) const;
	const float GetChildFloat( const std::string nodeName ) const;
	const bool GetChildBool( const std::string nodeName ) const;
	
	void CheckForNodeErrors( const Engine::JSON::JSONNode * node, const std::string nodeName, const JSONType & expectedType ) const;
};

class ParseException : public std::runtime_error {
public:

	ParseException( unsigned int pos, char errorChar, std::string expectedString ) : 
		std::runtime_error( "Parse exception" ),
		m_ExpectedString( expectedString ),
		m_ErrorPosition( pos ),
		m_ErrorChar( errorChar ) 
	{

	}

public:
	// The error character
	const char m_ErrorChar;
	// Text for the error
	const std::string m_ExpectedString;
	// The position of an error that stopped parsing
	const unsigned int m_ErrorPosition;
};

class JSONCastException : public std::runtime_error
{
public:
	JSONCastException( const std::string nodeName, const JSONType expectedType  ) :
		std::runtime_error( "Error loading level, missing value" ),
		m_NodeName( nodeName ),
		m_ExpectedType( expectedType ),
		m_FoundType( JSONType::Null ),
		m_NodeIndex( 0 )
	{

	}

	JSONCastException( const std::string nodeName, const JSONType expectedType, const JSONType foundType  ) :
		std::runtime_error( "Error loading level, data type mismatch" ),
		m_NodeName( nodeName ),
		m_ExpectedType( expectedType ),
		m_FoundType( foundType ),
		m_NodeIndex( 0 )
	{

	}

	JSONCastException( const unsigned int index, const JSONType expectedType  ) :
		std::runtime_error( "Error loading level, missing value" ),
		m_NodeIndex( index ),
		m_ExpectedType( expectedType ),
		m_FoundType( JSONType::Null ),
		m_NodeName( "" )
	{

	}

	JSONCastException( const unsigned int index, const JSONType expectedType, const JSONType foundType  ) :
		std::runtime_error( "Error loading level, data type mismatch" ),
		m_NodeIndex( index ),
		m_ExpectedType( expectedType ),
		m_FoundType( foundType ),
		m_NodeName( "" )
	{

	}

	const std::string m_NodeName;   // for object types;
	const JSONType m_ExpectedType;
	const JSONType m_FoundType;
	const unsigned int m_NodeIndex; // For array types
};

}
}