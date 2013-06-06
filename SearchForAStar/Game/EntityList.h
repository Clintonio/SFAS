// List of entities that are also indexed by type for faster access
// backed by a multimap

#pragma once

#include <map>
#include "Entity.h"

using std::multimap;

namespace SFAS
{
namespace Game
{
class EntityList : public multimap<const Entity::EntityType, const Entity*>
{
public:
	EntityList();
	~EntityList();

	const multimap<const Entity::EntityType, const Entity*>::iterator insert(const Entity * e);
private:

};
}
}