#include "egpch.h"
#include "AudioSystem.h"
#include "Engine/Scene/Components.h"
#include "Engine/Audio/SoundEngine.h"
#include "Util.h"


using TransformComponent	= Engine::Component::Core::TransformComponent;

using Sound3DComponent		= Engine::Component::Audio::Sound3DComponent;
using ListenerComponent		= Engine::Component::Audio::ListenerComponent;

namespace Engine::System::Audio {

	void OnUpdate(entt::registry& registry)
	{
		auto listeners = registry.view<ListenerComponent, TransformComponent>();
		if (listeners.begin() != listeners.end())
		{
			auto& [listenerComp, transformComp] = listeners.get<ListenerComponent, TransformComponent>(*listeners.begin());
			glm::vec4 direction = glm::toMat4(glm::quat(transformComp.Rotation)) * glm::vec4(0, 0, 1, 1);

			SoundEngine::SetListenerPosition(transformComp.Translation, { direction.x, direction.y, direction.z });
		}

		auto view = registry.view<Sound3DComponent, TransformComponent>();
		for (const entt::entity e : view)
		{
			auto& [soundComp, transformComp] = view.get<Sound3DComponent, TransformComponent>(e);

			soundComp.Sound->setPosition({ transformComp.Translation.x, transformComp.Translation.y, transformComp.Translation.z });
		}
	}

}
