//
// Source files for Search For A Star competition
// Copyright (c) 2013 L. Attwood
//
// Entity is the base class for all objects with a position and a velocity.
// 
// Add a summary of your changes here:
// - Made entity IDs generate internally for better encapsulation
// - Gave entities ability to perceive the world (ie; the world object) during update routine
//   for more intelligent behaviours
// - Removed RenderItem from the Render call
#pragma once

#include <d3dx9.h>
#include "Graphics/TextureLoader.h"

namespace Engine { class RenderItem; class TextRenderer; }

namespace SFAS
{

namespace Game                  
{          
	const float kGravity = 0.0f;
	class World;

class Entity
{
public:
	Entity( const D3DXVECTOR3& pos, const D3DXVECTOR3& scale, float damping );
	virtual ~Entity(void);

	struct EntityType 
	{
		EntityType(int type): typeID(type) {}

		const int typeID;

		inline bool operator< (const EntityType rhs) const 
		{
			return (typeID < rhs.typeID);
		}

		inline bool operator==(const EntityType rhs) const 
		{
			return (typeID == rhs.typeID);
		}
	};

	// Render and Update the Entity object 
	virtual void Render( );
	virtual void RenderDebug( Engine::TextRenderer * txt );
	virtual void Update( World * world, float dt );

	// Get the moveable position, velocity and orientation
	const D3DXVECTOR3& GetPosition() const { return m_Position; }
	const D3DXVECTOR3& GetVelocity() const { return m_Velocity; }
	const D3DXVECTOR3& GetScale() const { return m_Scale; }
	const D3DXVECTOR3& GetAcceleration() const { return m_Acceleration; }
	const D3DXVECTOR3& GetAccelerationLastFrame() const { return m_LastFrameAcceleration; } 

	// Set the moveable position, velocity or add force
	void SetPosition( const D3DXVECTOR3 & vec ) { m_Position = vec; }
	void SetVelocity( const D3DXVECTOR3 & vec ) { m_Velocity = vec; }
	void AddForce( const D3DXVECTOR3 & force ) { m_ForceAccumulator += force; }

	// Are we active and set active status 
	bool IsActive() const { return m_Active; }
	void SetActive( bool active ) { m_Active = active; m_TimeActive = 0.0f; }

	// Get the amount of time this Entity object has spent active
	float GetTimeSpentActive() const { return m_TimeActive; }

	// Collision
	void SetRadius( float radius ) { m_Radius = radius; } 

	bool CheckForPossibleCollision( const Entity& other, float dt );
	bool CheckForCollision( const Entity& other, float dt );
	// Get absolute distance to other entity center
	const float DistanceTo( const Entity * other) const;
	// Make this entity face a given direction
	void FaceDirection( const D3DXVECTOR2 direction );
	// Make this entity face a given direction, deletes the z component
	void FaceDirection( const D3DXVECTOR3 direction );

	float GetMass() const;

	virtual void OnCollision( Entity& other, World * world );

	virtual void OnReset() {
		m_ForceAccumulator = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
		m_Velocity = D3DXVECTOR3( 0,0,0);
	}

	bool IsMoveable() const { return m_InverseMass > 0.0f; }
	
	void SetCollidable( const bool collidable ) { m_Collide = collidable; }
	void SetCollidable( ) { m_Collide = true; }
	bool IsCollidable() const { return m_Collide; }

	void Resolve(Entity& other, float duration);

	int GetID() const { return m_ID; }

	virtual bool IsPlayerControlled() const { return false; }

	// Get the entity type for this entity
	virtual const Entity::EntityType GetEntityType() = 0;

	static void Init(LPDIRECT3DDEVICE9 p_dx_Device, HWND han_Window);
protected:

	void SetInverseMass( float iv ) { m_InverseMass = iv; }
	void SetMass( float mass );

	static bool Intersection( const D3DXVECTOR3 & p1, const D3DXVECTOR3 & p2, const D3DXVECTOR3 & s1, const D3DXVECTOR3 & s2, D3DXVECTOR3 & contact, float & penetration ); 
	
	void SetScale( D3DXVECTOR3 scale ) { m_Scale = scale; }

	// Return unit direction to the other entity
	const D3DXVECTOR3 DirectionToEntity( const Entity * other ) const;
	// The render item for this entity
	const Engine::RenderItem * m_RenderItem;
	// The texture loading for render items
	static Engine::TextureLoader* sTextureLoader;
private:

	virtual WCHAR * ToString()  const = 0;

	float CalculateSeparatingVelocity( Entity& other );
    void ResolveVelocity(float duration);
	void ResolveInterpenetration(float duration);
       
	void LinearIntegration( float dt );


	D3DXVECTOR3		m_Position;
	D3DXVECTOR3		m_Velocity;
	D3DXVECTOR3		m_Acceleration;
	D3DXVECTOR3		m_LastFrameAcceleration;
	D3DXVECTOR3		m_ForceAccumulator;
	D3DXVECTOR3		m_Scale;
	D3DXVECTOR3		m_ContactNormal;
	D3DXVECTOR3		m_ContactPoint;
	D3DXVECTOR3		m_Movement;
	D3DXVECTOR3		m_PossibleCollisionDirection;
	Entity *		m_Contact;
	float			m_PossibleCollisionDirectionLength;
	float			m_Radius;
	float			m_Damping;
	float			m_InverseMass;
	float			m_TimeActive;
	float			m_Restitution;
	float			m_Penetration;
	float			m_Friction;
	float			m_Tolerance;
	float			m_RotationAngle;
	int				m_ID;
	bool			m_Active;
	bool			m_Collide;

	static int		sHighestID;
};
}
}     