// Mini JSON parser for level loading
// I only implemented this because of the middleware ban

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

class JSONParser
{
public:
	JSONParser();
	
	// Error handling methods
	const unsigned int GetErrorPosition() const { return m_ErrorPosition; }
	const bool HasError() const { return m_Error; }
	const std::string GetExpectedString() const { return m_ExpectedString; }
	const char GetErrorChar() const { return m_ErrorChar; }
	const std::string GetErrorMessage() const;

	// This returns the final, parsed, tree to the user (0 if fail or none)
	const JSONMapNode * GetParsedTree() const { return m_RootNode; }
	// Generate and return the final, parsed, tree. 0 if fail or none.
	const JSONMapNode *  ParseJSONFile( const std::string file );
private:
	enum JSONSymbol 
	{
		String,
		Integer,
		Array,
		ArrayEnd,
		Object,
		ObjectEnd,
		Error,
		WhiteSpace,
		Character,
		Escape,
		MapSeparator,
		Number,
		True,
		False,
		Null,
		VariableSeparator,
		Dot,
		Negative,
		Positive,
		Exponent
	};

	std::string LoadJSONFile( const std::string file ) const;

	JSONSymbol		ParseSymbol( const char c );
	JSONMapNode *	ParseObject( const std::string json, unsigned int & cur, const unsigned int index );
	JSONArrayNode *	ParseArray( const std::string json, unsigned int & cur, const unsigned int index );
	JSONNode *		ParseVariable( const std::string json, unsigned int & cur, const unsigned int index );
	JSONNode *		ParseNumber( const std::string json, unsigned int & cur, const unsigned int index );
	JSONNode *		ParseString( const std::string json, unsigned int & cur, const unsigned int index );
	JSONNode *		ParseTrue( const std::string json, unsigned int & cur, const unsigned int index );
	JSONNode *		ParseFalse( const std::string json, unsigned int & cur, const unsigned int index );
	JSONNode *		ParseNull( const std::string json, unsigned int & cur, const unsigned int index );
	bool			ParseMapSeparator( const std::string json, unsigned int & cur, const unsigned int index );
	bool			ParseVariableSeparator( const std::string json, unsigned int & cur, const unsigned int index );
	
	inline void SetError( unsigned int pos, char errorChar, std::string expectedString ) { 
		m_ExpectedString = expectedString;
		m_ErrorPosition = pos; 
		m_ErrorChar = errorChar;
		m_Error = true; 
	}

	// The root node of our parsed tree
	JSONMapNode * m_RootNode;

	// The error character
	char m_ErrorChar;
	// Text for the error
	std::string m_ExpectedString;
	// The position of an error that stopped parsing
	unsigned int m_ErrorPosition;
	// True if parsing failed
	bool m_Error;
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