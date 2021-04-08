#pragma once
#include <cstdint>
#include <irrKlang.h>

struct MonsterComponent
{
	float Hitpoints;
	float Damage;
	float Speed;
	float ViewRange;
	float HitRange;

	irrklang::ISoundSource* LiveSound;
	irrklang::ISoundSource* DeathSound;

	MonsterComponent(irrklang::ISoundSource* liveSound = nullptr, irrklang::ISoundSource* deathSound = nullptr,
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

	irrklang::ISoundSource* CastSound = nullptr;
	irrklang::ISoundSource* ThrowSound = nullptr;
	irrklang::ISoundSource* ImpactSound = nullptr;

	MagicBallComponent(float effect = 20.0, float mana = 10.0f, float castTime = 2.0f)
		: Effect(effect), Mana(mana), CastTime(castTime) {}
	MagicBallComponent(irrklang::ISoundSource* castSound, irrklang::ISoundSource* throwSound, irrklang::ISoundSource* impactSound, float effect = 20.0, float mana = 10.0f, float castTime = 2.0f)
		: CastSound(castSound), ThrowSound(throwSound), ImpactSound(impactSound), Effect(effect), Mana(mana), CastTime(castTime) { }

	MagicBallComponent(const MagicBallComponent&) = default;
};

struct HeroComponent
{
	float Hitpoints;
	float Mana;
	float LockedMana = 0;

	HeroComponent(float hitpoints = 100.0f, float mana = 100.0f)
		: Hitpoints(hitpoints), Mana(mana) { }

	HeroComponent(const HeroComponent&) = default;
};
