#pragma once

#include <irrKlang.h>

namespace Engine::Component::Audio {

	// 2D Sound
	struct Sound2DComponent
	{
		std::string SoundSource;
		irrklang::ISound* Sound = nullptr;

		bool Loop;
		float Volume;

		Sound2DComponent(std::string soundSource = std::string(), bool loop = false, float volume = 0.5)
			: SoundSource(soundSource), Loop(loop), Volume(volume) {}

		Sound2DComponent(const Sound2DComponent&) = default;
	};

	// 3D Sound
	struct Sound3DComponent
	{
		std::string SoundSource;
		irrklang::ISound* Sound = nullptr;

		bool Loop;
		float Volume;

		Sound3DComponent(std::string soundSource = std::string(), bool loop = false, float volume = 0.5)
			: SoundSource(soundSource), Loop(loop), Volume(volume) {}

		Sound3DComponent(const Sound3DComponent&) = default;
	};

	// Listener
	struct ListenerComponent
	{
		uint16_t temp = 0; // TODO: fix (remove)

		ListenerComponent() = default;
		ListenerComponent(const ListenerComponent&) = default;
	};
}