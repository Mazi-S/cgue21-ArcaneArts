#include "egpch.h"
#include "ScriptableEntitySystem.h"
#include "Engine/Scene/Components.h"

namespace Engine::System::ScriptableEntity {

	void OnUpdate(entt::registry& registry, Timestep ts)
	{
		registry.view<Component::Core::NativeScriptComponent>().each([=](auto entity, auto& nsc) { if (nsc.Active) nsc.Instance->OnUpdate(ts); });
	}

}
