#pragma once
#include "Engine.h"
#include <cstdint>

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

	float Lifetime = 100.0f;

	std::string CastSound;
	std::string ThrowSound;
	std::string ImpactSound;

	MagicBallComponent(float effect = 20.0, float mana = 10.0f, float castTime = 2.0f)
		: Effect(effect), Mana(mana), CastTime(castTime) {}
	MagicBallComponent(std::string castSound, std::string throwSound, std::string impactSound, float effect = 20.0, float mana = 10.0f, float castTime = 2.0f)
		: CastSound(castSound), ThrowSound(throwSound), ImpactSound(impactSound), Effect(effect), Mana(mana), CastTime(castTime) { }

	MagicBallComponent(const MagicBallComponent&) = default;
};

struct HeroComponent
{
	float Hitpoints = 100.0f;
	float Mana = 100.0f;
	float LockedMana = 0;

	HeroComponent() = default;
	HeroComponent(const HeroComponent&) = default;
};
