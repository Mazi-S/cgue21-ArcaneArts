#pragma once

#include "Scene.h"

namespace Engine {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);
		~SceneSerializer() = default;

		void Serialize(const std::string& filepath);
		void Deserialize(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;
	};

}