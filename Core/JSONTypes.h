// Types for the JSON parser

#pragma once

#include <string>
#include <stdexcept>

namespace Engine
{
namespace JSON
{
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

	// Search through child tags for the given tag and
	// return that child
	const JSONNode * operator [] ( const std::string tagName ) const 
	{
		for( unsigned int i = 0; i < childCount; i++ )
		{
			if( (*childTags[i]) == tagName )
			{
				return child[i];
			}
		}

		return 0;
	}
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
}
}