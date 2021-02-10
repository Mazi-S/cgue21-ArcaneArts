#pragma once
#include "Camera.h"

namespace Engine {

	class Skybox
	{
	public:
		Skybox();
		~Skybox();

		void Draw(Camera camera);
	private:
		unsigned int cubeVAO, cubeVBO, skyboxVAO, skyboxVBO, cubemapTexture;
	};
}
