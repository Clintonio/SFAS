// List of entities that are also indexed by type for faster access
// backed by a multimap

#pragma once

#include <map>
#include <vector>
#include "Entity.h"

using std::multimap;

namespace SFAS
{
namespace Game
{
class EntityList : public multimap<const Entity::EntityType, Entity*>
{
public:
	EntityList();
	~EntityList();

	const multimap<const Entity::EntityType, Entity*>::iterator insert(Entity * e);

	const std::vector<Entity *> GetAllEntitiesOfType(const Entity::EntityType & type) const;
private:

};
}
}