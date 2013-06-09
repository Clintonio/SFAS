#include "JSONParser.h"
#include <vector>
#include <sstream>

using namespace Engine;

JSONParser::JSONParser() :
	m_ErrorPosition( 0 ),
	m_Error( false )
{

}

const std::string JSONParser::GetErrorMessage() const
{
	std::stringstream ss;
	ss << "Parser error: Expecting ";
	ss << m_ExpectedString;
	ss << ", but got: ";
	ss << m_ErrorChar;
	ss << " at string index ";
	ss << m_ErrorPosition;

	return ss.str();
}

void JSONParser::ParseJSONFile( const std::string file )
{
	std::string json = LoadJSONFile( file );
	
	JSONSymbol curSymbol;
	unsigned int length = json.length();
	unsigned int cur = 0;

	try {
		curSymbol = ParseSymbol( json[cur++] );
		if( curSymbol == JSONSymbol::Object )
		{
			ParseObject( json, cur, length );
		}
		else
		{
			throw ParseException( cur, json[cur], "{" );
		}
	} 
	catch ( ParseException e )
	{
		SetError( e.m_ErrorPosition, e.m_ErrorChar, e.m_ExpectedString );
	}
}

std::string JSONParser::LoadJSONFile( const std::string file ) const 
{
	FILE *filePtr = NULL;
	fopen_s( &filePtr, file.c_str(), "r" );
	fseek( filePtr, 0, SEEK_END );
	int size = ftell( filePtr );

	char* input = new char[size];
	fseek( filePtr, 0, 0 );
	fread( input, sizeof( char ), size, filePtr );

	fclose(filePtr);

	return input;
}

JSONParser::JSONSymbol JSONParser::ParseSymbol( const char c )
{
	switch ( c )
	{
		case '{':
			return JSONSymbol::Object;
		break;
		
		case '}':
			return JSONSymbol::ObjectEnd;
		break;

		case '[':
			return JSONSymbol::Array;
		break;

		case ']':
			return JSONSymbol::ArrayEnd;
		break;
		
		case '"':
			return JSONSymbol::String;
		break;
		
		case '\t':
		case ' ':
		case '\n':
			return JSONSymbol::WhiteSpace;
		break;

		case '\\':
			return JSONSymbol::Escape;
		break;
		
		case ':':
			return JSONSymbol::MapSeparator;
		break;
		
		case 't':
			return JSONSymbol::True;
		break;
		
		case 'f':
			return JSONSymbol::False;
		break;
		
		case 'n':
			return JSONSymbol::Null;
		break;

		case ',':
			return JSONSymbol::VariableSeparator;
		break;
		
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return JSONSymbol::Number;
		break;

		case 'e':
		case 'E':
			return JSONSymbol::Exponent;
		break;

		case '-':
			return JSONSymbol::Negative;
		break;

		case '.':
			return JSONSymbol::Dot;
		break;


		default:
			return JSONSymbol::Character;
	}
}

void JSONParser::ParseObject( const std::string json, unsigned int & cur, const unsigned int length )
{
	// Whether a variable separator has been encountered
	bool allowAnother = true;
	bool done = false;
	JSONSymbol curSymbol;
	while( cur < length && !done )
	{
		curSymbol = ParseSymbol( json[cur++] );
		if( curSymbol == JSONSymbol::String )
		{
			if( allowAnother )
			{
				std::string variableName = ParseString( json, cur, length );
				if( ParseMapSeparator( json, cur, length ) )
				{
					char curChar = json[cur];
					ParseVariable( json, ++cur, length );
					allowAnother = ParseVariableSeparator( json, cur, length );
				}
				else
				{
					done = true;
					throw ParseException( cur, json[cur], ":" );
				}
			}
			else
			{
				done = true;
				throw ParseException( cur, json[cur], "}" );
			}
		}
		else if( curSymbol == JSONSymbol::ObjectEnd )
		{
			if( allowAnother )
			{
				throw ParseException( cur, json[cur], "," );
			}
			done = true;
		}
		else if( curSymbol != JSONSymbol::WhiteSpace )
		{
			throw ParseException( cur, json[cur], "\"" );
			done = true;
		}

	}
}

std::string JSONParser::ParseString( const std::string json, unsigned int & cur, const unsigned int length )
{
	bool done = false;
	// Whether escape control is active
	bool escape = false;
	// Temporary character storage
	std::vector<char> string;
	char curChar;

	JSONSymbol curSymbol;
	while( cur < length && !done )
	{
		curChar = json[cur++];
		curSymbol = ParseSymbol( curChar );
		if( curSymbol == JSONSymbol::String && !escape )
		{
			done = true;
		}
		else if( curSymbol == JSONSymbol::Escape )
		{
			escape = !escape;
			// This means that we've seen "\\" 
			if( !escape )
			{
				string.push_back( curChar );
			}
		}
		else
		{
			string.push_back( curChar );
			escape = false;
		}
	}

	return std::string( string.begin(), string.end() );
}

bool JSONParser::ParseMapSeparator( const std::string json, unsigned int & cur, const unsigned int length )
{
	bool done = false;
	bool result = false;
	JSONSymbol curSymbol;
	while( cur < length && !done )
	{
		curSymbol = ParseSymbol( json[cur++] );
		if( curSymbol == JSONSymbol::MapSeparator )
		{
			done = true;
			result = true;
		}
		else if( curSymbol != JSONSymbol::WhiteSpace )
		{
			throw ParseException( cur, json[cur], ":" );
			done = true;
		}
	}

	return result;
}

bool JSONParser::ParseVariableSeparator( const std::string json, unsigned int & cur, const unsigned int length )
{
	bool done = false;
	bool result = false;
	JSONSymbol curSymbol;
	while( cur < length && !done )
	{
		curSymbol = ParseSymbol( json[cur] );
		if( curSymbol == JSONSymbol::VariableSeparator )
		{
			done = true;
			result = true;
			cur++;
		}
		else if( curSymbol != JSONSymbol::WhiteSpace )
		{
			done = true;
		}
		else
		{
			cur++;
		}
		
	}

	return result;
}

void JSONParser::ParseVariable( const std::string json, unsigned int & cur, const unsigned int length )
{
	bool done = false;
	JSONSymbol curSymbol;
	while( cur < length && !done )
	{
		done = true;
		curSymbol = ParseSymbol( json[cur++] );
		if( curSymbol == JSONSymbol::String )
		{
			ParseString( json, cur, length );
		}
		else if( curSymbol == JSONSymbol::Object )
		{
			ParseObject( json, cur, length );
		}
		else if( curSymbol == JSONSymbol::Array )
		{
			ParseArray( json, cur, length );
		}
		else if( curSymbol == JSONSymbol::Number || curSymbol == JSONSymbol::Negative)
		{
			ParseNumber( json, --cur, length );
		}
		else if( curSymbol == JSONSymbol::True )
		{
			ParseTrue( json, cur, length );
		}
		else if( curSymbol == JSONSymbol::False )
		{
			ParseFalse( json, cur, length );
		}
		else if( curSymbol == JSONSymbol::Null )
		{
			ParseNull( json, cur, length );
		}
		else if( curSymbol != JSONSymbol::WhiteSpace )
		{
			cur--;
			throw ParseException( cur, json[cur], "variable" );
			done = false;
		} 
		else
		{
			done = false;
		}
	}

}

void JSONParser::ParseArray( const std::string json, unsigned int & cur, const unsigned int length )
{
	bool done = false;
	// Whether to allow another variable (ie; a separator has been encountered)
	bool allowAnother = true;
	JSONSymbol curSymbol;
	while( cur < length && !done )
	{
		curSymbol = ParseSymbol( json[cur++] );
		if( curSymbol == JSONSymbol::ArrayEnd )
		{
			if( allowAnother )
			{
				throw ParseException( cur, json[cur], "variable" );
			} 
			done = true;
		}
		else if( curSymbol != JSONSymbol::WhiteSpace )
		{
			if( allowAnother )
			{
				--cur;
				ParseVariable( json, cur, length );
				allowAnother = ParseVariableSeparator( json, cur, length );
			}
			else
			{
				throw ParseException( cur, json[cur], "]" );
				done = true;
			}
		}
	}
}

// This method returns immediately after error
void JSONParser::ParseNumber( const std::string json, unsigned int & cur, const unsigned int length )
{
	bool negative = false;
	bool done = false;
	bool hasRightSide = false;
	bool exponent = false;
	JSONSymbol curSymbol;
	std::vector<char> leftSide;
	std::vector<char> rightSide;
	std::vector<char> exponentPart;
	char curChar;

	curSymbol = ParseSymbol( json[cur] );
	if( curSymbol == JSONSymbol::Negative )
	{
		negative = true;
		cur++;
	}

	while( cur < length && !done )
	{
		curChar = json[cur];
		curSymbol = ParseSymbol( curChar );
		if( curSymbol == JSONSymbol::Number )
		{
			leftSide.push_back( curChar );
			cur++;
		}
		else
		{
			done = true;
		}
	}

	curSymbol = ParseSymbol( json[cur] );
	if( curSymbol == JSONSymbol::Dot )
	{
		if( leftSide.size() > 0 )
		{
			hasRightSide = true;
			cur++;
		}
		else
		{
			done = true;
			throw ParseException( cur, json[cur], "digit" );
			return;
		}
	}

	done = hasRightSide;
	while( cur < length && !done )
	{
		curSymbol = ParseSymbol( json[cur] );
		if( curSymbol == JSONSymbol::Number )
		{
			rightSide.push_back( json[cur] );
			cur++;
		}
		else
		{
			done = true;
		}
	}

	curSymbol = ParseSymbol( json[cur] );
	if( curSymbol == JSONParser::Exponent )
	{
		if(( leftSide.size() > 0 ) || ( hasRightSide && rightSide.size() > 0 ))
		{
			exponent = true;
			cur++;
		} 
		else
		{
			done = true;
			throw ParseException( cur, json[cur], "digit" );
			return;
		}
	}
	
	done = exponent;
	while( cur < length && !done )
	{
		curSymbol = ParseSymbol( json[cur] );
		if( curSymbol == JSONSymbol::Number )
		{
			exponentPart.push_back( json[cur] );
			cur++;
		}
		else
		{
			done = true;
		}
	}


}

bool JSONParser::ParseTrue( const std::string json, unsigned int & cur, const unsigned int length )
{
	if( cur + 3 < length && json[cur + 1] == 'r' && json[cur + 2] == 'u' && json[cur + 3] == 'e')
	{
		return true;
	}
	else
	{
		throw ParseException( cur, json[--cur], "true" );
		return false;
	}
}

bool JSONParser::ParseFalse( const std::string json, unsigned int & cur, const unsigned int length )
{
	if( cur + 4 < length && json[cur + 1] == 'a' && json[cur + 2] == 'l' && json[cur + 3] == 's' && json[cur + 4] == 'e')
	{
		return true;
	}
	else
	{
		throw ParseException( cur, json[--cur], "true" );
		return false;
	}
}

bool JSONParser::ParseNull( const std::string json, unsigned int & cur, const unsigned int length )
{
	if( cur + 3 < length && json[cur + 1] == 'u' && json[cur + 2] == 'l' && json[cur + 3] == 'l')
	{
		return true;
	}
	else
	{
		throw ParseException( cur, json[--cur], "true" );
		return false;
	}
}