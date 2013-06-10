#include "JSONParser.h"
#include <vector>
#include <sstream>

using namespace Engine::JSON;

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

const JSONMapNode * JSONParser::ParseJSONFile( const std::string file )
{
	m_RootNode = 0; // Reset the root for this new parsed JSON

	std::string json = LoadJSONFile( file );
	if( json.length() > 0 )
	{
		JSONSymbol curSymbol;
		unsigned int length = json.length();
		unsigned int cur = 0;

		try {
			curSymbol = ParseSymbol( json[cur++] );
			if( curSymbol == JSONSymbol::Object )
			{
				m_RootNode = ParseObject( json, cur, length );
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

	return m_RootNode;
}

std::string JSONParser::LoadJSONFile( const std::string file ) const 
{
	char * input	= NULL;
	FILE *filePtr	= NULL;
	fopen_s( &filePtr, file.c_str(), "r" );
	if( filePtr != NULL )
	{
		fseek( filePtr, 0, SEEK_END );
		int size = ftell( filePtr );

		input = new char[size];
		fseek( filePtr, 0, 0 );
		fread( input, sizeof( char ), size, filePtr );

		fclose(filePtr);
	}
	else
	{
		input = "";
	}

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

		case '+':
			return JSONSymbol::Positive;
		break;

		case '.':
			return JSONSymbol::Dot;
		break;


		default:
			return JSONSymbol::Character;
	}
}

JSONNode * JSONParser::ParseVariable( const std::string json, unsigned int & cur, const unsigned int length )
{
	bool done = false;
	JSONSymbol curSymbol;
	while( cur < length && !done )
	{
		done = true;
		curSymbol = ParseSymbol( json[cur++] );
		if( curSymbol == JSONSymbol::String )
		{
			return ParseString( json, cur, length );
		}
		else if( curSymbol == JSONSymbol::Object )
		{
			return ParseObject( json, cur, length );
		}
		else if( curSymbol == JSONSymbol::Array )
		{
			return ParseArray( json, cur, length );
		}
		else if( curSymbol == JSONSymbol::Number || curSymbol == JSONSymbol::Negative)
		{
			return ParseNumber( json, --cur, length );
		}
		else if( curSymbol == JSONSymbol::True )
		{
			return ParseTrue( json, cur, length );
		}
		else if( curSymbol == JSONSymbol::False )
		{
			return ParseFalse( json, cur, length );
		}
		else if( curSymbol == JSONSymbol::Null )
		{
			return ParseNull( json, cur, length );
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

	return NULL;
}

JSONMapNode * JSONParser::ParseObject( const std::string json, unsigned int & cur, const unsigned int length )
{
	// Output data before processing
	std::vector<JSONNode*> data;
	// Tags for data
	std::vector<std::string*> tags;
	// Temporary storage for new child nodes
	JSONNode * child;
	// Processed output data
	JSONMapNode * out = new JSONMapNode;

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
				tags.push_back((std::string*) ParseString( json, cur, length )->value);
				if( ParseMapSeparator( json, cur, length ) )
				{
					char curChar = json[cur];
					child = ParseVariable( json, ++cur, length );
					child->parent = out;
					data.push_back( child );
					// Check if we can take another variable in this map
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

	out->childCount = data.size();
	out->childTags	= new std::string*[out->childCount];
	out->child		= new JSONNode*[out->childCount];
	out->type		= JSONType::Object;

	// Populate the child fields
	for( unsigned int i = 0; i < out->childCount; i++ )
	{
		out->childTags[i]	= tags[i];
		out->child[i]		= data[i];
	}

	return out;
}

JSONArrayNode * JSONParser::ParseArray( const std::string json, unsigned int & cur, const unsigned int length )
{
	JSONArrayNode * out = new JSONArrayNode;
	// Temporary storage for new child nodes
	JSONNode * child;
	// Temporary data store for unknown amounts of data
	std::vector<JSONNode*> data;
	// Whether to allow another variable (ie; a separator has been encountered)
	bool allowAnother = true;

	bool done = false;
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
				child = ParseVariable( json, cur, length );
				child->parent = out;
				data.push_back( child );
				// Check if we have a separator for another variable
				allowAnother = ParseVariableSeparator( json, cur, length );
			}
			else
			{
				throw ParseException( cur, json[cur], "]" );
				done = true;
			}
		}
	}

	out->type		= JSONType::Array;
	out->childCount = data.size();
	out->child		= new JSONNode*[out->childCount];

	// Populate the child fields
	for( unsigned int i = 0; i < out->childCount; i++ )
	{
		out->child[i] = data[i];
	}
	return out;
}

JSONNode * JSONParser::ParseString( const std::string json, unsigned int & cur, const unsigned int length )
{
	JSONNode * out;
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

	out			= new JSONNode;
	out->type	= JSONType::String;
	out->value	= (void *) new std::string( string.begin(), string.end() );
	return out;
}

JSONNode * JSONParser::ParseNumber( const std::string json, unsigned int & cur, const unsigned int length )
{
	// The output data
	JSONNode * out;
	JSONSymbol curSymbol;
	// Flags to determine the kind of number we have
	bool negative		= false;
	bool done			= false;
	bool hasRightSide	= false;
	bool exponent		= false;
	bool exponentSign	= true; // true is positive here
	// The data storage for the various parts of a number
	std::vector<char> leftSide;
	std::vector<char> rightSide;
	std::vector<char> exponentPart;
	char curChar;
	// Below are variables relating to the transformation 
	// to C data types
	int leftValue;
	std::stringstream buffer;


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
		}
	}

	done = !hasRightSide;
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
		}
	}
	
	done = !exponent;
	while( cur < length && !done )
	{
		curSymbol = ParseSymbol( json[cur] );
		if( curSymbol == JSONSymbol::Number )
		{
			exponentPart.push_back( json[cur] );
			cur++;

			// Check for the sign of the exponent
			if( curSymbol == JSONSymbol::Negative )
			{
				exponentSign = false;
				cur++;
			} 
			else if( curSymbol == JSONSymbol::Positive )
			{
				exponentSign = true;
				cur++;
			}
		}
		else
		{
			done = true;
		}
	}
	
	out	= new JSONNode;
	if( hasRightSide || exponent )
	{
		buffer << std::string( leftSide.begin(), leftSide.end() );
		if( hasRightSide )
		{
			buffer << ".";
			buffer << std::string( rightSide.begin(), rightSide.end() );
		}

		if( exponent )
		{
			buffer << "e";
			buffer << std::string( exponentPart.begin(), exponentPart.end() );
		}

		float* values	= new float[1];
		std::string str = buffer.str();
		values[0]		= (float) atof( str.c_str() );

		out->type		= JSONType::Float;
		out->value		= values;
	}
	else
	{
		int* values	= new int[1];
		leftValue	= ( negative ? -1 : 1 ) * atoi( std::string( leftSide.begin(), leftSide.end() ).c_str() );
		values[0]	= leftValue;

		out->type	= JSONType::Int;
		out->value	= values;
	}

	return out;
}

JSONNode * JSONParser::ParseTrue( const std::string json, unsigned int & cur, const unsigned int length )
{
	JSONNode * node;
	bool * data;
	if( cur + 3 < length && json[cur] == 'r' && json[cur + 1] == 'u' && json[cur + 2] == 'e')
	{
		cur = cur + 3;
		node = new JSONNode;
		data = new bool[1];
		data[0] = true;

		node->type  = JSONType::Bool;
		node->value = data;

		return node;
	}
	else
	{
		throw ParseException( cur, json[--cur], "true" );
	}
}

JSONNode * JSONParser::ParseFalse( const std::string json, unsigned int & cur, const unsigned int length )
{
	JSONNode * node;
	bool * data;
	if( cur + 4 < length && json[cur] == 'a' && json[cur + 1] == 'l' && json[cur + 2] == 's' && json[cur + 3] == 'e')
	{
		cur = cur + 4;
		node = new JSONNode;
		data = new bool[1];
		data[0] = false;

		node->type  = JSONType::Bool;
		node->value = data;

		return node;
	}
	else
	{
		throw ParseException( cur, json[--cur], "false" );
	}
}

JSONNode * JSONParser::ParseNull( const std::string json, unsigned int & cur, const unsigned int length )
{
	JSONNode * node;
	if( cur + 3 < length && json[cur] == 'u' && json[cur + 1] == 'l' && json[cur + 2] == 'l')
	{
		cur = cur + 3;
		node = new JSONNode;
		node->type	= JSONType::Null;
		node->value = NULL;
		return node;
	}
	else
	{
		throw ParseException( cur, json[--cur], "null" );
	}
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