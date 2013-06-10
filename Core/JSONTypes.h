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
};

struct JSONMapNode : public JSONArrayNode
{
	std::string	**	childTags; // Only in object

	// Search through child tags for the given tag and return that child
	const JSONNode * JSONMapNode::operator [] ( const std::string tagName ) const;
	const std::string JSONMapNode::GetChildString( const std::string nodeName ) const;
	const std::wstring JSONMapNode::GetChildWString( const std::string nodeName ) const;
	const int JSONMapNode::GetChildInt( const std::string nodeName ) const;
	const D3DXVECTOR3 JSONMapNode::GetChildVector3( const std::string nodeName ) const;
	const float GetChildFloat( const std::string nodeName ) const;
	
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
	JSONCastException( std::string nodeName, JSONType expectedType  ) :
		std::runtime_error( "Error loading level, missing value" ),
		m_NodeName( nodeName ),
		m_ExpectedType( expectedType )
	{

	}

	JSONCastException( std::string nodeName, JSONType expectedType, JSONType foundType  ) :
		std::runtime_error( "Error loading level, data type mismatch" ),
		m_NodeName( nodeName ),
		m_ExpectedType( expectedType ),
		m_FoundType( foundType )
	{

	}

	std::string m_NodeName;
	JSONType m_ExpectedType;
	JSONType m_FoundType;
};

}
}