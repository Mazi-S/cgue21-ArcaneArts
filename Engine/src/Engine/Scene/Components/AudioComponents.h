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

		Sound2DComponent(const Sound2DComponent& other)
			: SoundSource(other.SoundSource), Sound(nullptr), Loop(other.Loop), Volume(other.Volume) { }
	};

	// 3D Sound
	struct Sound3DComponent
	{
		std::string SoundSource;
		irrklang::ISound* Sound = nullptr;

		bool Loop;
		bool Paused;
		float Volume;

		Sound3DComponent(std::string soundSource = std::string(), bool loop = false, bool paused = false, float volume = 0.5)
			: SoundSource(soundSource), Loop(loop), Paused(paused), Volume(volume) {}

		Sound3DComponent(const Sound3DComponent& other)
			: SoundSource(other.SoundSource), Sound(nullptr), Loop(other.Loop), Paused(other.Paused), Volume(other.Volume) { }
	};

	// Listener
	struct ListenerComponent
	{
		uint16_t temp = 0; // TODO: fix (remove)

		ListenerComponent() = default;
		ListenerComponent(const ListenerComponent&) = default;
	};
}