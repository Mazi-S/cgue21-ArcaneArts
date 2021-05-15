#include "Engine/ImGui/SceneHierarchyPanel.h"
#include "Engine/ImGui/ImGuiUtil.h"
#include <imgui/imgui.h>

#include "GameComponents.h"

void Engine::SceneHierarchyPanel::DrawAddGameComponent(Entity entity)
{
	std::string component = "add...";
	std::vector<std::string> components;

	if (!entity.HasComponent<MonsterComponent>())
		components.push_back("Monster");
	if (!entity.HasComponent<HeroComponent>())
		components.push_back("Hero");

	if (components.size() > 0 && ImGuiUtil::DrawComboControl("Add Game Comp.", component, components))
	{
		if (component == "Monster")
			entity.AddComponent<MonsterComponent>();
		if (component == "Hero")
			entity.AddComponent<HeroComponent>();
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
}