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

const multimap<const Entity::EntityType, const Entity*>::iterator EntityList::insert(const Entity * e) 
{
	std::pair<const Entity::EntityType, const Entity*> pair(e->GetEntityType(), e);

	return multimap::insert(pair);
}