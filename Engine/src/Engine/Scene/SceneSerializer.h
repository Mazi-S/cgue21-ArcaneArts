#pragma once
#include "Scene.h"
#include "Entity.h"

#include <yaml-cpp/yaml.h>

namespace Engine {

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);
		~SceneSerializer() = default;

		void Serialize(const std::string& filepath);
		void Deserialize(const std::string& filepath);

		// implemented on client side (Game)
		void SerializeGameComponents(YAML::Emitter& out, Entity entity);
		void DeserializeGameComponents(Entity deserializedEntity, const YAML::Node& entityNode);

	private:
		void SerializeComponents(YAML::Emitter& out, Entity entity);
		void DeserializeComponents(Entity deserializedEntity, const YAML::Node& entityNode);

	private:
		Ref<Scene> m_Scene;
	};

}