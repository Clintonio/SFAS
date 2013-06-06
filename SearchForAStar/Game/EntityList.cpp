#include "EntityList.h"
#include <map>

using namespace SFAS::Game;
using std::multimap;


EntityList::EntityList() : multimap()
{
}
EntityList::~EntityList() 
{

}

const multimap<const Entity::EntityType, Entity*>::iterator EntityList::insert(Entity * e) 
{
	std::pair<const Entity::EntityType, Entity*> pair(e->GetEntityType(), e);

	return multimap::insert(pair);
}

const std::vector<Entity *> EntityList::GetAllEntitiesOfType(const Entity::EntityType & type) const {
	// Render the players
	std::pair<EntityList::const_iterator,EntityList::const_iterator> found = equal_range(type);
	EntityList::const_iterator start = found.first;
	EntityList::const_iterator end = found.second;

	std::vector<Entity*> out;
	while(start != end) 
	{
		Entity* entity = (*start).second;
		out.push_back(entity);
		start++;
	}

	return out;
}