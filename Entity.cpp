#include "Entity.h"

void Entity::addComponent(char* name, Component c)
{
	components[name] = c;
}

Component Entity::getComponent(char* name)
{
	return components[name];
}