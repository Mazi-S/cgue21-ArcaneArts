#include "WalkingSoundScript.h"
#include "Components/GameComponents.h"

using CharacterControllerComponent = Engine::Component::Physics::CharacterControllerComponent;

void WalkingSoundScript::OnUpdate(Engine::Timestep ts)
{
	auto [walkingSoundComp, characterControllerComp] = m_RegistryHandle->try_get<WalkingSoundComponent, CharacterControllerComponent>(m_EntityHandle);

	if (walkingSoundComp == nullptr || walkingSoundComp->WalkingSound == std::string() || characterControllerComp == nullptr)
		return;

	if (walkingSoundComp->Sound == nullptr)
	{
		Engine::Ref<Engine::Audio::SoundSource> source = Engine::SoundLibrary::Get(walkingSoundComp->WalkingSound);
		walkingSoundComp->Sound = source->Play2D(true, false, true);
	}

	if (characterControllerComp->Jumping || !characterControllerComp->Walking)
	{
		walkingSoundComp->Sound->setVolume(0);
	}
	else if (characterControllerComp->Running)
	{
		walkingSoundComp->Sound->setVolume(0.3);
		walkingSoundComp->Sound->setPlaybackSpeed(1.5);
	}
	else if (characterControllerComp->Crouching)
	{
		walkingSoundComp->Sound->setVolume(0.1);
		walkingSoundComp->Sound->setPlaybackSpeed(0.5);

	}
	else
	{
		walkingSoundComp->Sound->setVolume(0.2);
		walkingSoundComp->Sound->setPlaybackSpeed(1.0);
	}
}
