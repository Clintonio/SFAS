//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Entity is the base class for all objects with a position and a velocity.
// 
// Add a summary of your changes here:
// - Improved code in CheckForCollision by making it fit coding standards more
// - Improved collision functionality by making the checks use the full bounding box
// 

#include "Entity.h"

#include <wchar.h>
#include "float.h"
#include "Graphics/RenderItem.h"
#include "Graphics/TextRenderer.h"

using SFAS::Game::Entity;

Entity::Entity( int id, const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, float damping ) : m_Position(pos), 
																		m_Velocity( 0.0f, 0.0f, 0.0f ), 
																		m_Acceleration( 0.0f, 0.0f, 0.0f ), 
																		m_LastFrameAcceleration( 0.0f, 0.0f, 0.0f ), 
																		m_ForceAccumulator( 0.0f, 0.0f, 0.0f ), 
																		m_Scale( scale ), 
																		m_ContactNormal( 0.0f, 0.0f, 0.0f ),
																		m_ContactPoint( 0.0f, 0.0f, 0.0f ),
																		m_Movement( 0.0f, 0.0f, 0.0f ),
																		m_Contact( 0 ),
																		m_Radius( max( scale.x, scale.y ) * 0.5f ),
																		m_Damping(damping), 
																		m_InverseMass(0.0f),
																		m_TimeActive(0.0f), 
																		m_Restitution(0.0f),
																		m_Penetration(0.0f),
																		m_Friction(0.0f),
																		m_Tolerance(0.0f),
																		m_ID( id ),
																		m_Active(false),
																		m_Collide(false)
{
}


Entity::~Entity(void)
{
}

void Entity::Render( Engine::RenderItem * drw )
{
	if( m_Active )
	{
		D3DXMATRIX world;
		D3DXMATRIX move;
		D3DXMATRIX scale;

		D3DXMatrixTranslation( &move, m_Position.x, m_Position.y, m_Position.z );
		D3DXMatrixScaling( &scale, m_Scale.x, m_Scale.y, m_Scale.z );
		
		world = scale * move;
		drw->Draw( &world );
	}
}

void Entity::RenderDebug( Engine::TextRenderer * txt )
{
	WCHAR strBuffer[512];
	swprintf_s( strBuffer, 512, L"ID %d %s {%f, %f}", m_ID, ToString(), m_Position.x, m_Position.y );
	txt->DrawDebug( strBuffer, 800, 100 + ( m_ID * 10 ), D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
}

void Entity::Update( float dt )
{
	if( m_Active )
	{
		m_ForceAccumulator.y -= ( kGravity * GetMass() );

		LinearIntegration( dt );

		// Count this frame
		m_TimeActive += dt;
	}
}

bool Entity::CheckForPossibleCollision( const Entity& other, float dt )
{
	// Possible collision if the two particles are within each other's movement
	// radius during the timestep.
	float movementDistance = (D3DXVec3Length(&m_Velocity) + D3DXVec3Length(&other.GetVelocity())) * dt;
	float minimumDistance = m_Radius + other.m_Radius + movementDistance;
	D3DXVECTOR3 difference = m_Position - other.m_Position;
	float length = D3DXVec3Length( &difference );

	if( minimumDistance > length )
	{
		D3DXVec3Normalize( &m_PossibleCollisionDirection, &difference );
		m_PossibleCollisionDirectionLength = length;
		return true;
	}

	return false;
}

bool Entity::CheckForCollision( const Entity& other, float dt )
{
	if( ( m_Position.x - (m_Scale.x * 0.5f) ) > ( other.m_Position.x + ( other.m_Scale.x * 0.5f ) ) ) 
	{ 
		return false; // This is on the right of other
	}

	if( ( m_Position.y - (m_Scale.y * 0.5f) ) > ( other.m_Position.y + ( other.m_Scale.y * 0.5f ) ) ) 
	{
		return false; // This is under other
	}

	if( ( other.m_Position.x - (other.m_Scale.x * 0.5f) ) > ( m_Position.x + ( m_Scale.x * 0.5f ) ) ) 
	{
		return false; // Other is on the right of this
	}

	if( ( other.m_Position.y - (other.m_Scale.y * 0.5f) ) > ( m_Position.y + ( m_Scale.y * 0.5f ) ) ) 
	{
		return false; // Other is under this
	}
	
	return true;
}

void Entity::OnCollision( Entity& other )
{
	// Default behavour
	SetActive( false );
}

void Entity::LinearIntegration( float dt )
{
	// Don't Integrate invalid particles
	if (m_InverseMass <= 0.0f) 
	{
		return;
	}

	// Update current position with velocity
	m_Position += ( m_Velocity * dt );

	// Calculate acceleration
    m_LastFrameAcceleration = m_Acceleration;
    m_LastFrameAcceleration += ( m_ForceAccumulator * m_InverseMass );

	// Calculate velocity
	m_Velocity += ( m_LastFrameAcceleration * dt );

	// Damp the velocity to slow the object over time
	m_Velocity *= pow(m_Damping, dt);

	// Save and clear forces
	m_ForceAccumulator.x = m_ForceAccumulator.y = m_ForceAccumulator.z = 0.0f;
}

void Entity::SetMass( float mass)
{
    m_InverseMass = ( 1.0f / mass );
}

float Entity::GetMass() const
{
    if( m_InverseMass == 0 )
    {
        return FLT_MAX;
    }
    else 
    {
        return (1.0f / m_InverseMass );
    }
}

void Entity::Resolve(Entity& other, float duration)
{
	m_Contact = &other;
	m_Velocity = m_Velocity * -1.0f;
}

