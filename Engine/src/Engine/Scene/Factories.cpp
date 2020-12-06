#include "egpch.h"
#include "Factories.h"
#include "Systems.h"
#include "Components.h"

namespace Engine::Factory {

	entt::entity CreateEntity(entt::registry& registry, const std::string& name)
	{
		entt::entity entity = registry.create();
		registry.emplace<TransformComponent>(entity);
		registry.emplace<TagComponent>(entity, name.empty() ? "Entity" : name);
		return entity;
	}

	entt::entity CreateHero(entt::registry& registry)
	{
		entt::entity hero = registry.create();
		registry.emplace<TransformComponent>(hero);
		registry.emplace<TagComponent>(hero, "Hero");
		registry.emplace<HeroComponent>(hero);
		registry.emplace<CharacterControllerComponent>(hero);
		System::CharacterController::Activate(registry, hero);
		return hero;
	}

	entt::entity CreateCamera(entt::registry& registry, entt::entity parent, glm::vec3 offset, glm::vec3 rotation)
	{
		entt::entity camera = registry.create();
		auto& tc = registry.emplace<TransformComponent>(camera);
		tc.Translation = offset; tc.Rotation = rotation;
		registry.emplace<TagComponent>(camera, "Camera");
		registry.emplace<CameraComponent>(camera);
		if(parent != entt::null)
			registry.emplace<ParentComponent>(camera, parent);

		return camera;
	}

	entt::entity CreateMagicBall(entt::registry& registry)
	{
		entt::entity ball = CreateEntity(registry, "Magic Ball");
		registry.emplace<MaterialComponent>(ball, MaterialLibrary::Get("MagicBall"));
		registry.emplace<MeshComponent>(ball, MeshLibrary::Get("Sphere"));
		registry.emplace<ParentComponent>(ball, hero);

		auto& tc = registry.get<TransformComponent>(ball);
		tc.Scale = { 0.2f, 0.2f, 0.2f };
		if (mainHand)
		{
			// right hand
			tc.Translation = { 0.5f, -0.2f, -1.5f };
			if (hc->RightHand != entt::null)
				registry.destroy(hc->RightHand);
			hc->RightHand = ball;
		}
		else
		{
			// left hand
			tc.Translation = { -0.5f, -0.2f, -1.5f };
			if (hc->LeftHand != entt::null)
				registry.destroy(hc->LeftHand);
			hc->LeftHand = ball;
		}

		return ball;
	}
}