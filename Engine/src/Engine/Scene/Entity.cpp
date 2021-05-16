#include "egpch.h"
#include "Entity.h"
#include "Components/CoreComponents.h"
#include "Factories.h"

namespace Engine {

	Entity::Entity(entt::entity handle, entt::registry* registry)
		: m_EntityHandle(handle), m_Registry(registry)
	{ }

	const uint32_t Entity::GetID() const
	{
		using Identifier = Engine::Component::Core::Identifier;

		if (m_Registry == nullptr || m_EntityHandle == entt::null || !HasComponent<Identifier>())
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

	Entity Entity::Copy()
	{
		entt::entity copy = Factory::Copy(*m_Registry, m_EntityHandle);
		return Entity(copy, m_Registry);
	}

	std::string Entity::ToString() const
	{
		std::stringstream ss;

		ss << "Entity{";

		if (HasComponent<Component::Core::TagComponent>())
		ss << "Tag:" << m_Registry->get<Component::Core::TagComponent>(m_EntityHandle).Tag << ",";
		ss << "ID:" << m_Registry->get<Component::Core::Identifier>(m_EntityHandle).ID;

		ss << "}";
		return ss.str();
	}
}