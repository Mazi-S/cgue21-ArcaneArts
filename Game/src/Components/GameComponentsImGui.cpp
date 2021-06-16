#include "Engine/ImGui/SceneHierarchyPanel.h"
#include "Engine/ImGui/ImGuiUtil.h"
#include <imgui/imgui.h>

#include "GameComponents.h"
#include "Entities/PointLightFlickerScript.h"
#include "Entities/WalkingSoundScript.h"
#include "Entities/ItemScript.h"

void Engine::SceneHierarchyPanel::DrawAddGameComponent(Entity entity)
{
	std::string component = "add...";
	std::vector<std::string> components;

	if (!entity.HasComponent<MonsterComponent>())
		components.push_back("Monster");
	if (!entity.HasComponent<HeroComponent>())
		components.push_back("Hero");
	if (!entity.HasComponent<PointLightFlickerComponent>())
		components.push_back("PointLightFlicker");
	if (!entity.HasComponent<WalkingSoundComponent>())
		components.push_back("WalkingSound");
	if (!entity.HasComponent<HealthItemComponent>())
		components.push_back("HealthItem");
	if (!entity.HasComponent<ManaItemComponent>())
		components.push_back("ManaItem");

	if (components.size() > 0 && ImGuiUtil::DrawComboControl("Add Game Comp.", component, components))
	{
		if (component == "Monster")
			entity.AddComponent<MonsterComponent>();
		if (component == "Hero")
			entity.AddComponent<HeroComponent>();
		if (component == "PointLightFlicker")
		{
			if (entity.HasComponent<Engine::Component::Renderer::PointLightComponent>())
				entity.AddComponent<PointLightFlickerComponent>(entity.GetComponent<Engine::Component::Renderer::PointLightComponent>().Color);
			else
				entity.AddComponent<PointLightFlickerComponent>();

			entity.AddNativeScript<PointLightFlickerScript>();
		}
		if (component == "WalkingSound")
		{
			entity.AddComponent<WalkingSoundComponent>();
			entity.AddNativeScript<WalkingSoundScript>();
		}
		if (component == "HealthItem")
		{
			entity.AddComponent<HealthItemComponent>();
			entity.AddNativeScript<ItemScript>();
		}
		if (component == "ManaItem")
		{
			entity.AddComponent<ManaItemComponent>();
			entity.AddNativeScript<ItemScript>();
		}
	}
}

void Engine::SceneHierarchyPanel::DrawGameComponents(Entity entity)
{
	// Monster Component
	ImGuiUtil::DrawComponent<MonsterComponent>("Monster", entity, [](auto& component)
		{
			std::string liveSound = component.LiveSound;
			if (ImGuiUtil::DrawComboControl("LiveSound", liveSound, SoundLibrary::GetNames()))
				component.LiveSound = liveSound;

			std::string deathSound = component.DeathSound;
			if (ImGuiUtil::DrawComboControl("DeathSound", deathSound, SoundLibrary::GetNames()))
				component.DeathSound = deathSound;

			ImGuiUtil::DrawFloatControl("Hitpoints", component.Hitpoints, 0, 2000, 1);
			ImGuiUtil::DrawFloatControl("Damage", component.Damage, 0, 100, 1);
			ImGuiUtil::DrawFloatControl("Speed", component.Speed, 0, 100, 0.001);
			ImGuiUtil::DrawFloatControl("ViewRange", component.ViewRange, 10, 1000, 1);
			ImGuiUtil::DrawFloatControl("HitRange", component.HitRange, 0, 100, 0.1);

			ImGui::Dummy({ 0, .5 });
			ImGuiUtil::Text("Description", "Defines the entity as a monster.");
	});

	// Hero Component
	ImGuiUtil::DrawComponent<HeroComponent>("Hero", entity, [](auto& component)
		{
			ImGuiUtil::Text("Hitpoints", std::to_string(component.Hitpoints));
			ImGuiUtil::Text("Mana", std::to_string(component.Mana));
			ImGuiUtil::Text("LockedMana", std::to_string(component.LockedMana));

			ImGui::Dummy({ 0, .5 });
			ImGuiUtil::Text("Description", "Defines the entity as the hero.");
	});

	// Point Light Flicker
	ImGuiUtil::DrawComponent<PointLightFlickerComponent>("PointLightFlicker", entity, [](PointLightFlickerComponent& component)
		{
			ImGuiUtil::DrawFloat3Control("Color", component.Color);

			ImGui::Dummy({ 0, 0.3 });

			ImGuiUtil::DrawFloatControl("Duration", component.Duration, 0, 100);
			ImGuiUtil::DrawFloatControl("Variation (D)", component.DurationVariation, 0, 10);
			
			ImGui::Dummy({ 0, 0.3 });

			ImGuiUtil::DrawFloatControl("Intensity", component.Intensity, 0, 100);
			ImGuiUtil::DrawFloatControl("Variation (I)", component.IntensityVariation, 0, 10);

			ImGui::Dummy({ 0, 0.5 });

			ImGuiUtil::Text("TimeLeft", std::to_string(component.TimeLeft));
			std::stringstream ss;
			ss << component.IntensityDelta.x << " " << component.IntensityDelta.y << " " << component.IntensityDelta.z;
			ImGuiUtil::Text("IntensityDelta", ss.str());
		});

	// Walking Sound Component
	ImGuiUtil::DrawComponent<WalkingSoundComponent>("Walking Sound", entity, [](WalkingSoundComponent& component)
	{
		std::string soundSource = component.WalkingSound;
		if (ImGuiUtil::DrawComboControl("WalkingSound", soundSource, SoundLibrary::GetNames()))
		{
			component.WalkingSound = soundSource;
		}
			
	});

	// Health Item Component
	ImGuiUtil::DrawComponent<HealthItemComponent>("Health Item", entity, [](HealthItemComponent& component)
	{
		ImGuiUtil::Text("Description", "Specifies if the entity is a health item.");
	});

	// Mana Item Component
	ImGuiUtil::DrawComponent<ManaItemComponent>("Mana Item", entity, [](ManaItemComponent& component)
	{
		ImGuiUtil::Text("Description", "Specifies if the entity is a mana item.");
	});

}