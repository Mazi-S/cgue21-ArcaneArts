#include "egpch.h"
#include "SceneSerializer.h"

#include "Components.h"

#include "Engine/Util/Serialization.h"

namespace Engine {

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{ }

	
	void SceneSerializer::Serialize(const std::string& filepath)
	{
		LOG_INFO("Serialize Scene...\n\tfile: {}", filepath);

		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";

		if (m_Scene->m_MainCamera != entt::null)
		{
			Entity mainCamera = { m_Scene->m_MainCamera, &m_Scene->m_Registry };
			out << YAML::Key << "MainCamera" << YAML::Value << mainCamera.GetID();
		}

		out << YAML::Key << "Spectator" << YAML::Value << m_Scene->m_SpectatorActive;

		out << YAML::Key << "Entities" << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID)
			{
				Entity entity = { entityID, &m_Scene->m_Registry };
				if (!entity || entity.HasComponent<Component::Core::Unserializable>())
					return;

				out << YAML::BeginMap; // Entity
				out << YAML::Key << "Entity" << YAML::Value << entity.GetID();

				SerializeComponents(out, entity);
				SerializeGameComponents(out, entity);

				out << YAML::EndMap; // Entity

			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::Deserialize(const std::string& filepath)
	{
		LOG_INFO("Deserialize Scene...\n\tfile: {}", filepath);

		YAML::Node data = YAML::LoadFile(filepath);

		ASSERT(data["Scene"], "No Scene node!");

		if (data["Spectator"])
		{
			bool spectator = data["Spectator"].as<bool>();
			m_Scene->m_SpectatorActive = spectator;
		}

		if (data["Entities"])
		{
			std::map<uint32_t, Entity> associations;
			std::map<Entity, uint32_t> parents;

			for (YAML::Node entityNode : data["Entities"])
			{
				std::string name = entityNode["TagComponent"]
					? entityNode["TagComponent"]["Tag"].as<std::string>()
					: "Unknown Entity";
				uint32_t id = entityNode["Entity"].as<uint32_t>();

				Entity deserializedEntity = m_Scene->CreateEntity(name, id);

				// Association
				associations[id] = deserializedEntity;

				// Parent
				if (entityNode["ParentComponent"])
				{
					uint32_t parentID = entityNode["ParentComponent"]["Parent"].as<uint32_t>();
					parents[deserializedEntity] = parentID;
				}

				DeserializeComponents(deserializedEntity, entityNode);
				DeserializeGameComponents(deserializedEntity, entityNode);
			}

			if (data["MainCamera"])
			{
				uint32_t mainCameraID = data["MainCamera"].as<uint32_t>();

				if (associations[mainCameraID])
					m_Scene->SetMainCamera(associations[mainCameraID]);
			}

			for (auto& entry : parents)
			{
				uint32_t parentID = entry.second;
				Entity child = entry.first;

				if (associations[parentID])
					child.AddComponent<Engine::Component::Core::ParentComponent>(associations[parentID]);
			}

		}
	}

	void SceneSerializer::SerializeComponents(YAML::Emitter& out, Entity entity)
	{
		using TagComponent = Engine::Component::Core::TagComponent;
		using TransformComponent = Engine::Component::Core::TransformComponent;
		using ParentComponent = Engine::Component::Core::ParentComponent;

		using MeshComponent = Engine::Component::Renderer::MeshComponent;
		using MaterialComponent = Engine::Component::Renderer::MaterialComponent;
		using ShadowComponent = Engine::Component::Renderer::ShadowComponent;
		using DirectionalLightComponent = Engine::Component::Renderer::DirectionalLightComponent;
		using PointLightComponent = Engine::Component::Renderer::PointLightComponent;
		using CameraComponent = Engine::Component::Renderer::CameraComponent;

		using StaticColliderComponent = Engine::Component::Physics::StaticColliderComponent;
		using CharacterControllerComponent = Engine::Component::Physics::CharacterControllerComponent;

		using Sound2DComponent = Engine::Component::Audio::Sound2DComponent;
		using Sound3DComponent = Engine::Component::Audio::Sound3DComponent;
		using ListenerComponent = Engine::Component::Audio::ListenerComponent;

		// Tag
		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent

			auto& tagComp = entity.GetComponent<TagComponent>();
			out << YAML::Key << "Tag" << YAML::Value << tagComp.Tag;

			out << YAML::EndMap; // TagComponent
		}

		// Parent
		if (entity.HasComponent<ParentComponent>() && entity.GetComponent<ParentComponent>().Parent != entt::null)
		{
			out << YAML::Key << "ParentComponent";
			out << YAML::BeginMap; // ParentComponent

			Entity parent = entity.GetParent();
			out << YAML::Key << "Parent" << YAML::Value << parent.GetID();

			out << YAML::EndMap; // ParentComponent
		}

		// Transform
		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent

			auto& transformComp = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << transformComp.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << transformComp.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transformComp.Scale;

			out << YAML::EndMap; // TransformComponent
		}

		// Mesh
		if (entity.HasComponent<MeshComponent>())
		{
			out << YAML::Key << "MeshComponent";
			out << YAML::BeginMap; // MeshComponent

			auto& meshComp = entity.GetComponent<MeshComponent>();
			out << YAML::Key << "Mesh" << YAML::Value << meshComp.Mesh;

			out << YAML::EndMap; // MeshComponent
		}

		// Material
		if (entity.HasComponent<MaterialComponent>())
		{
			out << YAML::Key << "MaterialComponent";
			out << YAML::BeginMap; // MaterialComponent

			auto& materialComp = entity.GetComponent<MaterialComponent>();
			out << YAML::Key << "Material" << YAML::Value << materialComp.Material;

			out << YAML::EndMap; // MaterialComponent
		}

		// Shadow
		if (entity.HasComponent<ShadowComponent>())
		{
			out << YAML::Key << "ShadowComponent";
			out << YAML::Flow;
			out << YAML::BeginMap; // ShadowComponent
			out << YAML::EndMap; // ShadowComponent
		}

		// Directional Light
		if (entity.HasComponent<DirectionalLightComponent>())
		{
			out << YAML::Key << "DirectionalLightComponent";
			out << YAML::BeginMap; // DirectionalLightComponent

			auto& directionalLightComp = entity.GetComponent<DirectionalLightComponent>();
			out << YAML::Key << "Direction" << YAML::Value << directionalLightComp.Direction;
			out << YAML::Key << "Color" << YAML::Value << directionalLightComp.Color;

			out << YAML::EndMap; // DirectionalLightComponent
		}

		// Point Light
		if (entity.HasComponent<PointLightComponent>())
		{
			out << YAML::Key << "PointLightComponent";
			out << YAML::BeginMap; // PointLightComponent

			auto& pointLightComp = entity.GetComponent<PointLightComponent>();
			out << YAML::Key << "Color" << YAML::Value << pointLightComp.Color;
			out << YAML::Key << "Constant" << YAML::Value << pointLightComp.Constant;
			out << YAML::Key << "Linear" << YAML::Value << pointLightComp.Linear;
			out << YAML::Key << "Quadratic" << YAML::Value << pointLightComp.Quadratic;

			out << YAML::EndMap; // PointLightComponent
		}

		// Camera
		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComp = entity.GetComponent<CameraComponent>();
			out << YAML::Key << "FOV" << YAML::Value << cameraComp.FOV;
			out << YAML::Key << "Near" << YAML::Value << cameraComp.Near;
			out << YAML::Key << "Far" << YAML::Value << cameraComp.Far;

			out << YAML::EndMap; // CameraComponent
		}

		// Static Collider
		if (entity.HasComponent<StaticColliderComponent>())
		{
			out << YAML::Key << "StaticColliderComponent";
			out << YAML::Flow;
			out << YAML::BeginMap; // StaticColliderComponent
			out << YAML::EndMap; // StaticColliderComponent
		}

		// Character Controller
		if (entity.HasComponent<CharacterControllerComponent>())
		{
			out << YAML::Key << "CharacterControllerComponent";
			out << YAML::BeginMap; // CharacterControllerComponent

			auto& characterControllerComp = entity.GetComponent<CharacterControllerComponent>();

			out << YAML::Key << "Active" << YAML::Value << characterControllerComp.Active;
			out << YAML::Key << "TranslationSpeed" << YAML::Value << characterControllerComp.TranslationSpeed;
			out << YAML::Key << "RotationSpeed" << YAML::Value << characterControllerComp.RotationSpeed;

			out << YAML::Key << "StandingHeight" << YAML::Value << characterControllerComp.StandingHeight;
			out << YAML::Key << "CrouchingHeight" << YAML::Value << characterControllerComp.CrouchingHeight;
			out << YAML::Key << "Radius" << YAML::Value << characterControllerComp.Radius;

			out << YAML::EndMap; // CharacterControllerComponent
		}

		// Sound 2D
		if (entity.HasComponent<Sound2DComponent>())
		{
			out << YAML::Key << "Sound2DComponent";
			out << YAML::BeginMap; // Sound2DComponent

			auto& sound2DComp = entity.GetComponent<Sound2DComponent>();

			out << YAML::Key << "SoundSource" << YAML::Value << sound2DComp.SoundSource;
			out << YAML::Key << "Loop" << YAML::Value << sound2DComp.Loop;
			out << YAML::Key << "Volume" << YAML::Value << sound2DComp.Volume;

			out << YAML::EndMap; // Sound2DComponent
		}

		// Sound 3D
		if (entity.HasComponent<Sound3DComponent>())
		{
			out << YAML::Key << "Sound3DComponent";
			out << YAML::BeginMap; // Sound3DComponent

			auto& sound3DComp = entity.GetComponent<Sound3DComponent>();

			out << YAML::Key << "SoundSource" << YAML::Value << sound3DComp.SoundSource;
			out << YAML::Key << "Loop" << YAML::Value << sound3DComp.Loop;
			out << YAML::Key << "Volume" << YAML::Value << sound3DComp.Volume;

			out << YAML::EndMap; // Sound3DComponent
		}

		// Audio Listener
		if (entity.HasComponent<ListenerComponent>())
		{
			out << YAML::Key << "ListenerComponent";
			out << YAML::Flow;
			out << YAML::BeginMap; // ListenerComponent
			out << YAML::EndMap; // ListenerComponent
		}

	}

	void SceneSerializer::DeserializeComponents(Entity deserializedEntity, const YAML::Node& entityNode)
	{
		using TagComponent = Engine::Component::Core::TagComponent;
		using TransformComponent = Engine::Component::Core::TransformComponent;

		using MeshComponent = Engine::Component::Renderer::MeshComponent;
		using MaterialComponent = Engine::Component::Renderer::MaterialComponent;
		using ShadowComponent = Engine::Component::Renderer::ShadowComponent;
		using DirectionalLightComponent = Engine::Component::Renderer::DirectionalLightComponent;
		using PointLightComponent = Engine::Component::Renderer::PointLightComponent;
		using CameraComponent = Engine::Component::Renderer::CameraComponent;

		using StaticColliderComponent = Engine::Component::Physics::StaticColliderComponent;
		using CharacterControllerComponent = Engine::Component::Physics::CharacterControllerComponent;

		using Sound2DComponent = Engine::Component::Audio::Sound2DComponent;
		using Sound3DComponent = Engine::Component::Audio::Sound3DComponent;
		using ListenerComponent = Engine::Component::Audio::ListenerComponent;

		// Transform
		if (entityNode["TransformComponent"])
		{
			auto compNode = entityNode["TransformComponent"];
			auto& transformComp = deserializedEntity.GetComponent<TransformComponent>();
			transformComp.Translation = compNode["Translation"].as<glm::vec3>();
			transformComp.Rotation = compNode["Rotation"].as<glm::vec3>();
			transformComp.Scale = compNode["Scale"].as<glm::vec3>();
		}

		// Mesh
		if (entityNode["MeshComponent"])
		{
			auto compNode = entityNode["MeshComponent"];
			std::string mesh = compNode["Mesh"].as<std::string>();
			deserializedEntity.AddComponent<MeshComponent>(mesh);
		}

		// Material
		if (entityNode["MaterialComponent"])
		{
			auto compNode = entityNode["MaterialComponent"];
			std::string material = compNode["Material"].as<std::string>();
			deserializedEntity.AddComponent<MaterialComponent>(material);
		}

		// Shadow
		if (entityNode["ShadowComponent"])
		{
			deserializedEntity.AddComponent<ShadowComponent>();
		}

		// Directional Light
		if (entityNode["DirectionalLightComponent"])
		{
			auto compNode = entityNode["DirectionalLightComponent"];
			glm::vec3 direction = compNode["Direction"].as<glm::vec3>();
			glm::vec3 color = compNode["Color"].as<glm::vec3>();
			deserializedEntity.AddComponent<DirectionalLightComponent>(direction, color);
		}

		// Point Light
		if (entityNode["PointLightComponent"])
		{
			auto compNode = entityNode["PointLightComponent"];
			glm::vec3 color = compNode["Color"].as<glm::vec3>();
			float constant = compNode["Constant"].as<float>();
			float linear = compNode["Linear"].as<float>();
			float quadratic = compNode["Quadratic"].as<float>();
			deserializedEntity.AddComponent<PointLightComponent>(color, constant, linear, quadratic);
		}

		// Camera
		if (entityNode["CameraComponent"])
		{
			auto compNode = entityNode["CameraComponent"];
			float fov = compNode["FOV"].as<float>();
			float nearPlane = compNode["Near"].as<float>();
			float farPlane = compNode["Far"].as<float>();
			deserializedEntity.AddComponent<CameraComponent>(fov, nearPlane, farPlane);
		}

		// Shadow
		if (entityNode["StaticColliderComponent"])
		{
			deserializedEntity.AddComponent<StaticColliderComponent>();
		}

		// Character Controller
		if (entityNode["CharacterControllerComponent"])
		{
			auto compNode = entityNode["CharacterControllerComponent"];

			float standingHeight = compNode["StandingHeight"].as<float>();
			float crouchingHeight = compNode["CrouchingHeight"].as<float>();
			float radius = compNode["Radius"].as<float>();
			float translationSpeed = compNode["TranslationSpeed"].as<float>();
			float rotationSpeed = compNode["RotationSpeed"].as<float>();

			bool active = compNode["Active"].as<bool>();

			deserializedEntity.AddComponent<CharacterControllerComponent>(standingHeight, crouchingHeight, radius, translationSpeed, rotationSpeed, active);
		}

		// Sound 2D
		if (entityNode["Sound2DComponent"])
		{
			auto compNode = entityNode["Sound2DComponent"];

			std::string soundSource = compNode["SoundSource"].as<std::string>();
			bool loop = compNode["Loop"].as<bool>();
			float volume = compNode["Volume"].as<float>();

			deserializedEntity.AddComponent<Sound2DComponent>(soundSource, loop, volume);
		}

		// Sound 3D
		if (entityNode["Sound3DComponent"])
		{
			auto compNode = entityNode["Sound3DComponent"];

			std::string soundSource = compNode["SoundSource"].as<std::string>();
			bool loop = compNode["Loop"].as<bool>();
			float volume = compNode["Volume"].as<float>();

			deserializedEntity.AddComponent<Sound3DComponent>(soundSource, loop, volume);
		}

		// Listener
		if (entityNode["ListenerComponent"])
		{
			deserializedEntity.AddComponent<ListenerComponent>();
		}

	}

}