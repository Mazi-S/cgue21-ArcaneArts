#include "egpch.h"
#include "Entity.h"

namespace Engine {

	Entity::Entity(entt::entity handle, entt::registry* registry)
		: m_EntityHandle(handle), m_Registry(registry)
	{ }
}