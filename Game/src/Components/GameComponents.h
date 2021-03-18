#pragma once
#include <cstdint>
#include <irrKlang.h>

struct MonsterComponent
{
	float Hitpoints;
	float Damage;
	float Speed;
	float HitRange;

	MonsterComponent(float hitpoints = 100.0f, float damge = 20.0f, float speed = 2.0, float hitRange = 1.0f)
		: Hitpoints(hitpoints), Damage(damge), Speed(speed), HitRange(hitRange) { }
	MonsterComponent(const MonsterComponent&) = default;
};

struct MagicBallComponent
{
	float Damage;
	float Lifetime = 100.0f;

	irrklang::ISoundSource* CastSound = nullptr;
	irrklang::ISoundSource* ThrowSound = nullptr;
	irrklang::ISoundSource* ImpactSound = nullptr;

	MagicBallComponent(float damage = 20.0)
		: Damage(damage) {}
	MagicBallComponent(irrklang::ISoundSource* castSound, irrklang::ISoundSource* throwSound, irrklang::ISoundSource* impactSound, float damage = 20.0)
		: CastSound(castSound), ThrowSound(throwSound), ImpactSound(impactSound), Damage(damage) { }

	MagicBallComponent(const MagicBallComponent&) = default;
};
