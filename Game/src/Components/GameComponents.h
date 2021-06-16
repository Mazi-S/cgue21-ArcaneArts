#pragma once
#include "Engine.h"
#include <cstdint>
#include <irrKlang.h>

struct MonsterComponent
{
	std::string LiveSound;
	std::string DeathSound;

	float Hitpoints;
	float Damage;
	float Speed;
	float ViewRange;
	float HitRange;

	MonsterComponent(std::string liveSound = std::string(), std::string deathSound = std::string(),
		float hitpoints = 100.0f, float damge = 20.0f, float speed = 2.0, float viewRange = 22.0f, float hitRange = 3.0f)
		: LiveSound(liveSound), DeathSound(deathSound), Hitpoints(hitpoints), Damage(damge), Speed(speed), ViewRange(viewRange), HitRange(hitRange) { }

	MonsterComponent(const MonsterComponent&) = default;
};

struct MagicBallComponent
{
	float Effect;
	float Mana;
	float CastTime;

	float Progress = 0;

	float Lifetime = 5.0f;

	std::string CastSound;
	std::string ThrowSound;
	std::string ImpactSound;

	MagicBallComponent(float effect = 20.0, float mana = 10.0f, float castTime = 2.0f)
		: Effect(effect), Mana(mana), CastTime(castTime) {}
	MagicBallComponent(std::string castSound, std::string throwSound, std::string impactSound, float effect = 20.0, float mana = 10.0f, float castTime = 2.0f)
		: CastSound(castSound), ThrowSound(throwSound), ImpactSound(impactSound), Effect(effect), Mana(mana), CastTime(castTime) { }

	MagicBallComponent(const MagicBallComponent&) = default;
};

struct StoneComponent
{
	float Charge = 0.0f;

	StoneComponent() = default;
	StoneComponent(const StoneComponent&) = default;
};

struct HealthItemComponent
{
	uint16_t temp = 0;

	HealthItemComponent() = default;
	HealthItemComponent(const HealthItemComponent&) = default;
};

struct WalkingSoundComponent
{
	std::string WalkingSound;
	irrklang::ISound* Sound = nullptr;

	WalkingSoundComponent(std::string walkingSound = std::string())
		: WalkingSound(walkingSound) { }
	WalkingSoundComponent(const WalkingSoundComponent&) = default;
};

struct HeroComponent
{
	float Hitpoints = 100.0f;
	float Mana = 100.0f;
	float LockedMana = 0;

	HeroComponent() = default;
	HeroComponent(const HeroComponent&) = default;
};

struct PointLightFlickerComponent
{
	glm::vec3 Color;

	float Duration = 1;
	float DurationVariation = 1;
	float Intensity = 1.2f;
	float IntensityVariation = 0.2f;

	float TimeLeft = 0;
	glm::vec3 IntensityDelta = { 0, 0, 0 };
	
	PointLightFlickerComponent(glm::vec3 color = { 1.0f, 1.0f, 1.0f })
		: Color(color) { }

	PointLightFlickerComponent(glm::vec3 color, float duration, float durationVariation, float intensity, float intensityVariation)
		: Color(color), Duration(duration), DurationVariation(durationVariation), Intensity(intensity), IntensityVariation(intensityVariation) { }

	PointLightFlickerComponent(const PointLightFlickerComponent&) = default;
};
