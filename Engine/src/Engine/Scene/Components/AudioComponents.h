#pragma once

#include <irrKlang.h>

namespace Engine::Component::Audio {

	// 2D Sound
	struct Sound2DComponent
	{
		irrklang::ISoundSource* SoundSource = nullptr;
		irrklang::ISound* Sound = nullptr;
		bool Loop;

		Sound2DComponent(irrklang::ISoundSource* soundSource, bool loop = false)
			: SoundSource(soundSource), Loop(loop) {}

		Sound2DComponent(const Sound2DComponent&) = default;
	};

	// 3D Sound
	struct Sound3DComponent
	{
		irrklang::ISoundSource* SoundSource = nullptr;
		irrklang::ISound* Sound = nullptr;
		bool Loop;

		Sound3DComponent(irrklang::ISoundSource* soundSource, bool loop = false)
			: SoundSource(soundSource), Loop(loop) {}

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