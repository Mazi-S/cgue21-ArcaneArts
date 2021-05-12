#include "egpch.h"
#include "Entity.h"
#include "Components/CoreComponents.h"

namespace Engine {

	Entity::Entity(entt::entity handle, entt::registry* registry)
		: m_EntityHandle(handle), m_Registry(registry)
	{ }

	const uint32_t Entity::GetID() const
	{
		using Identifier = Engine::Component::Core::Identifier;

		if (!HasComponent<Identifier>())
			return 0;

		return m_Registry->get<Identifier>(m_EntityHandle).ID;
	}

	Entity Entity::GetParent()
	{
		using ParentComponent = Engine::Component::Core::ParentComponent;

		if(!HasComponent<ParentComponent>() || GetComponent<ParentComponent>().Parent == entt::null)
			return Entity();

		return {GetComponent<ParentComponent>().Parent, m_Registry};
	}

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