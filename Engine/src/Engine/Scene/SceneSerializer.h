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

	private:
		void SerializeComponents(YAML::Emitter& out, Entity entity);
		void DeserializeCoreComponents(Entity deserializedEntity, const YAML::Node& entityNode, std::map<uint32_t, Entity> entities);
		void DeserializeComponents(Entity deserializedEntity, const YAML::Node& entityNode, std::map<uint32_t, Entity> entities);

		// implemented on client side (game)
		void SerializeGameComponents(YAML::Emitter& out, Entity entity);
		void DeserializeGameComponents(Entity deserializedEntity, const YAML::Node& entityNode);

	private:
		Ref<Scene> m_Scene;
	};

}