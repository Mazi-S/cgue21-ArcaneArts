#include "egpch.h"
#include "Entity.h"
#include "Components/CoreComponents.h"

namespace Engine {

	Entity::Entity(entt::entity handle, entt::registry* registry)
		: m_EntityHandle(handle), m_Registry(registry)
	{ }

	std::string Entity::ToString() const
	{
		std::stringstream ss;

		ss << "Entity{";

		if (HasComponent<Component::Core::TagComponent>())
		ss << "Tag:" << m_Registry->get<Component::Core::TagComponent>(m_EntityHandle).Tag;

		ss << "}";
		return ss.str();
	}
}