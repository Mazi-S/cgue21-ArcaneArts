#include "Engine/Scene/SceneSerializer.h"
#include "GameComponents.h"

#include "Entities/HeroScript.h"
#include "Entities/MonsterScript.h"

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

}

void Engine::SceneSerializer::DeserializeGameComponents(Entity deserializedEntity, const YAML::Node& entityNode)
{
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

		deserializedEntity.AddComponent<MonsterComponent>(liveSound, deathSound, hitpoints, damage, speed, viewRange, hitRange);
		deserializedEntity.AddNativeScript<MonsterScript>();
	}

	// Hero Component
	if (entityNode["HeroComponent"])
	{
		deserializedEntity.AddComponent<HeroComponent>();
		deserializedEntity.AddNativeScript<HeroScript>();
	}
}