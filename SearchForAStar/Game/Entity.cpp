//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Entity is the base class for all objects with a position and a velocity.
// 
// Add a summary of your changes here:
// - Improved code in CheckForCollision by making it fit coding standards more
// - Improved collision functionality by making the checks use the full bounding box
// - Improved collision support by making collision inelastic.
// - Moved rendering code to render item

#include "Entity.h"

#include <wchar.h>
#include "float.h"
#include "Graphics/RenderItem.h"
#include "Graphics/TextRenderer.h"
#include "World.h"
#include <cmath>

using namespace SFAS::Game;

int Entity::sHighestID = 0;
Engine::TextureLoader* Entity::sTextureLoader = 0;

Entity::Entity( const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, float damping ) : m_Position(pos), 
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
																		m_Active(false),
																		m_Collide(true),
																		m_RotationAngle(0.0f)
{
	m_ID = sHighestID++;
}


Entity::~Entity(void)
{
}

void Entity::Render( )
{
	if( m_Active )
	{
		m_RenderItem->Draw( m_Position, m_Scale, m_RotationAngle );
	}
}

void Entity::RenderDebug( Engine::TextRenderer * txt )
{
	WCHAR strBuffer[512];
	swprintf_s( strBuffer, 512, L"ID %d %s {%f, %f}, %f", m_ID, ToString(), m_Position.x, m_Position.y, m_RotationAngle );
	txt->DrawDebug( strBuffer, 800, 100 + ( m_ID * 10 ), D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
}

void Entity::Update( World * world, float dt )
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

const D3DXVECTOR3 Entity::DirectionToEntity( const Entity * other ) const
{
	D3DXVECTOR3 direction = other->GetPosition() - GetPosition();
	D3DXVECTOR3 unitDir;
	D3DXVec3Normalize( &unitDir, &direction );

	return unitDir;
}

void Entity::FaceDirection( const D3DXVECTOR2 direction )
{
	m_RotationAngle = atan2(direction.y, direction.x);
}

void Entity::FaceDirection( const D3DXVECTOR3 direction )
{
	FaceDirection( D3DXVECTOR2( direction.x, direction.y ));
}

const float Entity::DistanceTo( const Entity * other) const
{
	const D3DXVECTOR3 direction = other->GetPosition() - GetPosition();

	return D3DXVec3Length(&direction);
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

bool Entity::OnCollision( Entity& other, World * world )
{
	// Default behavour
	SetActive( false );

	return true;
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
	float massA = GetMass();
	float massB = other.GetMass();
	D3DXVECTOR3 velA = GetVelocity();
	D3DXVECTOR3 velB = other.GetVelocity();

	// Calculate momentum changes for BOTH parties.
	// Info: http://en.wikipedia.org/wiki/Inelastic_collision
	float coeffOfRestitution = 0.5f;
	float nominatorXA = coeffOfRestitution * massB * (velB.x - velA.x) + massA*velA.x + massB*velB.x;
	float nominatorYA = coeffOfRestitution * massB * (velB.y - velA.y) + massA*velA.y + massB*velB.y;
	float nominatorXB = coeffOfRestitution * massA * (velA.x - velB.x) + massA*velA.x + massB*velB.x;
	float nominatorYB = coeffOfRestitution * massA * (velA.y - velB.y) + massA*velA.y + massB*velB.y;
	float denominator = massA + massB;

	m_Velocity.x = nominatorXA / denominator;
	m_Velocity.y = nominatorYA / denominator;
	m_Velocity.z = 0;

	other.SetVelocity(D3DXVECTOR3(
		nominatorXB / denominator, 
		nominatorYB / denominator, 
		0
	));
}

void Entity::Init(LPDIRECT3DDEVICE9 p_dx_Device)
{
	sTextureLoader = new Engine::TextureLoader(p_dx_Device);
}