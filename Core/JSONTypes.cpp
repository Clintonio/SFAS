#include "JSONTypes.h"

using namespace Engine::JSON;

// Search through child tags for the given tag and
// return that child
const JSONNode * JSONMapNode::operator [] ( const std::string tagName ) const 
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
	

const std::string JSONMapNode::GetObjectChildStringValue( const std::string nodeName ) const
{
	const JSONNode * child = (*this)[nodeName];
	CheckForNodeErrors( child, nodeName, JSONType::String );

	return *((std::string*) child->value);
}

const std::wstring JSONMapNode::GetObjectChildWStringValue( const std::string nodeName ) const
{
	const JSONNode * child = (*this)[nodeName];
	CheckForNodeErrors( child, nodeName, JSONType::String );

	std::string str = *((std::string*) child->value);
	return std::wstring( str.begin(), str.end() );
}

const int JSONMapNode::GetObjectChildIntValue( const std::string nodeName ) const
{
	const JSONNode * child = (*this)[nodeName];
	CheckForNodeErrors( child, nodeName, JSONType::Int );
	return *((int*) child->value);
}

const D3DXVECTOR3 JSONMapNode::GetObjectChildVector3Value( const std::string nodeName ) const
{
	JSONArrayNode * arrayNode = (JSONArrayNode*) (*this)[nodeName];
	CheckForNodeErrors( arrayNode, nodeName, JSONType::Array );
	CheckForNodeErrors( arrayNode->child[0], nodeName, JSONType::Float );
	CheckForNodeErrors( arrayNode->child[1], nodeName, JSONType::Float );
	CheckForNodeErrors( arrayNode->child[2], nodeName, JSONType::Float );
	float x = *((float*) (arrayNode->child[0])->value);
	float y = *((float*) (arrayNode->child[1])->value);
	float z = *((float*) (arrayNode->child[2])->value);

	return D3DXVECTOR3( x, y, z );
}

void JSONMapNode::CheckForNodeErrors( const Engine::JSON::JSONNode * node, const std::string nodeName, const JSONType & expectedType ) const
{
	if( node == 0 )
	{
		throw JSONCastException( nodeName, expectedType );
	}
	
	if ( node->type != expectedType )
	{
		throw JSONCastException( nodeName, expectedType, node->type );
	}
}