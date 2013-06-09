// Mini JSON parser for level loading
// I only implemented this because of the middleware ban

#pragma once

#include <string>
#include <stdexcept>

namespace Engine
{

class JSONParser
{
public:
	JSONParser();
	
	// Gets the position of the last error
	const unsigned int GetErrorPosition() const { return m_ErrorPosition; }
	const bool HasError() const { return m_Error; }
	const std::string GetExpectedString() const { return m_ExpectedString; }
	const char GetErrorChar() const { return m_ErrorChar; }
	const std::string GetErrorMessage() const;
	
	void ParseJSONFile( std::string file );
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
		Exponent
	};

	std::string LoadJSONFile( const std::string file ) const;

	JSONSymbol ParseSymbol( const char c );
	void ParseObject( const std::string json, unsigned int & cur, const unsigned int index );
	std::string ParseString( const std::string json, unsigned int & cur, const unsigned int index );
	bool ParseMapSeparator( const std::string json, unsigned int & cur, const unsigned int index );
	void ParseVariable( const std::string json, unsigned int & cur, const unsigned int index );
	void ParseArray( const std::string json, unsigned int & cur, const unsigned int index );
	bool ParseTrue( const std::string json, unsigned int & cur, const unsigned int index );
	bool ParseFalse( const std::string json, unsigned int & cur, const unsigned int index );
	bool ParseNull( const std::string json, unsigned int & cur, const unsigned int index );
	void ParseNumber( const std::string json, unsigned int & cur, const unsigned int index );
	bool ParseVariableSeparator( const std::string json, unsigned int & cur, const unsigned int index );
	
	inline void SetError( unsigned int pos, char errorChar, std::string expectedString ) { 
		m_ExpectedString = expectedString;
		m_ErrorPosition = pos; 
		m_ErrorChar = errorChar;
		m_Error = true; 
	}

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