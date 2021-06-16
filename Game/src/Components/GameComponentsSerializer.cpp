#include "Engine/Scene/SceneSerializer.h"

#include "Engine/Util/Serialization.h"
#include "Engine/Scene/Components.h"
#include "GameComponents.h"

#include "Entities/HeroScript.h"
#include "Entities/MonsterScript.h"
#include "Entities/PointLightFlickerScript.h"
#include "Entities/WalkingSoundScript.h"
#include "Entities/ItemScript.h"

#include "ActorFactory.h"

void Engine::SceneSerializer::SerializeGameComponents(YAML::Emitter& out, Entity entity)
{

	// Monster Component
	if (entity.HasComponent<MonsterComponent>())
	{
		out << YAML::Key << "MonsterComponent";
		out << YAML::BeginMap; // MonsterComponent

		auto& monsterComp = entity.GetComponent<MonsterComponent>();

		out << YAML::Key << "LiveSound" << YAML::Value << monsterComp.LiveSound;
		out << YAML::Key << "DeathSound" << YAML::Value << monsterComp.DeathSound;

		out << YAML::Key << "Hitpoints" << YAML::Value << monsterComp.Hitpoints;
		out << YAML::Key << "Damage" << YAML::Value << monsterComp.Damage;
		out << YAML::Key << "Speed" << YAML::Value << monsterComp.Speed;
		out << YAML::Key << "ViewRange" << YAML::Value << monsterComp.ViewRange;
		out << YAML::Key << "HitRange" << YAML::Value << monsterComp.HitRange;

		out << YAML::EndMap; // MonsterComponent
	}

	// Hero Component
	if (entity.HasComponent<HeroComponent>())
	{
		out << YAML::Key << "HeroComponent";
		out << YAML::Flow;
		out << YAML::BeginMap; // HeroComponent
		out << YAML::EndMap; // HeroComponent
	}

	// PLF
	if (entity.HasComponent<PointLightFlickerComponent>())
	{
		out << YAML::Key << "PointLightFlickerComponent";
		out << YAML::BeginMap; // PointLightFlickerComponent

		auto& flickerComp = entity.GetComponent<PointLightFlickerComponent>();

		out << YAML::Key << "Color" << YAML::Value << flickerComp.Color;
		out << YAML::Key << "Duration" << YAML::Value << flickerComp.Duration;
		out << YAML::Key << "DurationVariation" << YAML::Value << flickerComp.DurationVariation;
		out << YAML::Key << "Intensity" << YAML::Value << flickerComp.Intensity;
		out << YAML::Key << "IntensityVariation" << YAML::Value << flickerComp.IntensityVariation;

		out << YAML::EndMap; // PointLightFlickerComponent
	}

	// Walking Sound Component
	if (entity.HasComponent<WalkingSoundComponent>())
	{
		out << YAML::Key << "WalkingSoundComponent";
		out << YAML::BeginMap; // WalkingSoundComponent

		auto& walkingSoundComp = entity.GetComponent<WalkingSoundComponent>();

		out << YAML::Key << "Sound" << YAML::Value << walkingSoundComp.WalkingSound;

		out << YAML::EndMap; // WalkingSoundComponent
	}

	// Health Item
	if (entity.HasComponent<HealthItemComponent>())
	{
		out << YAML::Key << "HealthItemComponent";
		out << YAML::Flow;
		out << YAML::BeginMap; // HealthItemComponent
		out << YAML::EndMap; // HealthItemComponent
	}

}

void Engine::SceneSerializer::DeserializeGameComponents(Entity deserializedEntity, const YAML::Node& entityNode)
{
	using TransformComponent = Engine::Component::Core::TransformComponent;
	using MeshComponent = Engine::Component::Renderer::MeshComponent;

	// Monster Component
	if (entityNode["MonsterComponent"])
	{
		auto compNode = entityNode["MonsterComponent"];

		std::string liveSound = compNode["LiveSound"].as<std::string>();
		std::string deathSound = compNode["DeathSound"].as<std::string>();

		float hitpoints = compNode["Hitpoints"].as<float>();
		float damage = compNode["Damage"].as<float>();
		float speed = compNode["Speed"].as<float>();
		float viewRange = compNode["ViewRange"].as<float>();
		float hitRange = compNode["HitRange"].as<float>();

		if (deserializedEntity.HasComponent<MeshComponent>())
		{
			TransformComponent transformComp = Engine::System::Util::GlobalTransform(*deserializedEntity.m_Registry, deserializedEntity);
			MeshComponent& meshComp = deserializedEntity.GetComponent<MeshComponent>();
			auto actor = ActorFactory::Monster(meshComp.Mesh, transformComp.Translation, transformComp.Rotation, transformComp.Scale);
			deserializedEntity.AddComponent<Engine::Component::Physics::RigidDynamicComponent>(actor);
		}

		deserializedEntity.AddComponent<MonsterComponent>(liveSound, deathSound, hitpoints, damage, speed, viewRange, hitRange);
		deserializedEntity.AddNativeScript<MonsterScript>();
	}

	// Hero Component
	if (entityNode["HeroComponent"])
	{
		deserializedEntity.AddComponent<HeroComponent>();
		deserializedEntity.AddNativeScript<HeroScript>();

		if (deserializedEntity.HasComponent<TransformComponent>() && deserializedEntity.HasComponent<MeshComponent>())
		{
			auto& transformComp = deserializedEntity.GetComponent<TransformComponent>();
			auto& meshComp = deserializedEntity.GetComponent<MeshComponent>();
			auto actor = ActorFactory::Monster(meshComp.Mesh, transformComp.Translation, transformComp.Rotation, transformComp.Scale);
			deserializedEntity.AddComponent<Engine::Component::Physics::RigidDynamicComponent>(actor);
		}
	}

	// Hero Component
	if (entityNode["PointLightFlickerComponent"])
	{
		auto compNode = entityNode["PointLightFlickerComponent"];

		glm::vec3 color = compNode["Color"].as<glm::vec3>();
		float duration = compNode["Duration"].as<float>();
		float durationVariation = compNode["DurationVariation"].as<float>();
		float intensity = compNode["Intensity"].as<float>();
		float intensityVariation = compNode["IntensityVariation"].as<float>();

		deserializedEntity.AddComponent<PointLightFlickerComponent>(color, duration, durationVariation, intensity, intensityVariation);
		deserializedEntity.AddNativeScript<PointLightFlickerScript>();
	}

	// Hero Component
	if (entityNode["WalkingSoundComponent"])
	{
		auto compNode = entityNode["WalkingSoundComponent"];

		std::string sound = compNode["Sound"].as<std::string>();

		deserializedEntity.AddComponent<WalkingSoundComponent>(sound);
		deserializedEntity.AddNativeScript<WalkingSoundScript>();
	}

	// Health Item
	if (entityNode["HealthItemComponent"])
	{
		deserializedEntity.AddComponent<HealthItemComponent>();
		deserializedEntity.AddNativeScript<ItemScript>();
	}
}