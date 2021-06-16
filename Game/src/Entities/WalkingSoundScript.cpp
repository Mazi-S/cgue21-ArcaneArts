#include "WalkingSoundScript.h"
#include "Components/GameComponents.h"

using CharacterControllerComponent = Engine::Component::Physics::CharacterControllerComponent;

void WalkingSoundScript::OnCreate()
{
	auto* walkingSoundComp = m_RegistryHandle->try_get<WalkingSoundComponent>(m_EntityHandle);

	if (walkingSoundComp != nullptr && walkingSoundComp->WalkingSound != std::string())
	{
		Engine::Ref<Engine::Audio::SoundSource> source = Engine::SoundLibrary::Get(walkingSoundComp->WalkingSound);
		m_Sound = source->Play2D(true, false, true);
		m_Sound->setVolume(0);
	}
}

void WalkingSoundScript::OnDestroy()
{
	if (m_Sound != nullptr)
	{
		m_Sound->stop();
		m_Sound->drop();
		m_Sound = nullptr;
	}
}

void WalkingSoundScript::OnUpdate(Engine::Timestep ts)
{
	auto [walkingSoundComp, characterControllerComp] = m_RegistryHandle->try_get<WalkingSoundComponent, CharacterControllerComponent>(m_EntityHandle);

	if (walkingSoundComp == nullptr || walkingSoundComp->WalkingSound == std::string() || characterControllerComp == nullptr)
		return;

	if (m_Sound == nullptr)
	{
		Engine::Ref<Engine::Audio::SoundSource> source = Engine::SoundLibrary::Get(walkingSoundComp->WalkingSound);
		m_Sound = source->Play2D(true, false, true);
	}

	if (characterControllerComp->Jumping || !characterControllerComp->Walking)
	{
		m_Sound->setVolume(0);
	}
	else if (characterControllerComp->Running)
	{
		m_Sound->setVolume(0.3);
		m_Sound->setPlaybackSpeed(1.5);
	}
	else if (characterControllerComp->Crouching)
	{
		m_Sound->setVolume(0.1);
		m_Sound->setPlaybackSpeed(0.5);
	}
	else
	{
		m_Sound->setVolume(0.2);
		m_Sound->setPlaybackSpeed(1.0);
	}
}

