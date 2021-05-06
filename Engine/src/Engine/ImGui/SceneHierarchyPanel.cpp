#include "egpch.h"
#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include "ImGuiUtil.h"

#include <entt.hpp>

#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Mesh.h"

#include "Engine/Renderer/MaterialLibrary.h"
#include "Engine/Scene/Components.h"

namespace Engine {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
		: m_Context(context)
	{ }

	void SceneHierarchyPanel::OnImGui()
	{
		ImGui::Begin("Scene Hierarchy", &m_Active, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save")) { Save(); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		m_Context->m_Registry.each(
			[&](auto entityID)
			{
				DrawEntityNode(Entity(entityID, &m_Context->m_Registry));
			}
		);

		ImGui::End();


		ImGui::Begin("Entity");

		if (m_SelectionContext)
		{
			DrawEntity(m_SelectionContext);
		}
		else
		{
			if (ImGuiUtil::Button("Add Entity", glm::vec2{ -1, 0 }, ImGuiUtil::ButtonType::Success))
			{
				// TODO: implement...
				LOG_WARN("Add entity not implemented!");
			}
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<Engine::Component::Core::TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= (m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
			m_SelectionContext = m_SelectionContext != entity ? entity : Entity();

		if (opened)
			ImGui::TreePop();
	}

	void SceneHierarchyPanel::DrawEntity(Entity entity)
	{
		using TagComponent			= Engine::Component::Core::TagComponent;
		using TransformComponent	= Engine::Component::Core::TransformComponent;
		using MeshComponent			= Engine::Component::Renderer::MeshComponent;
		using MaterialComponent		= Engine::Component::Renderer::MaterialComponent;

		if (entity.HasComponent<TagComponent>())
		{
			auto tag = entity.GetComponent<TagComponent>().Tag;
			if (ImGuiUtil::InputText("Tag", tag, 100))
				entity.GetComponent<TagComponent>().Tag = tag;
		}

		std::string component = "add...";
		std::vector<std::string> components;

		if (!entity.HasComponent<TransformComponent>())
			components.push_back("Transform");

		if (components.size() > 0 && ImGuiUtil::DrawComboControl("Add Component", component, components))
		{
			if (component == "Transform")
				entity.AddComponent<TransformComponent>();
		}

		ImGui::Separator();

		// Transform Component
		ImGuiUtil::DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				glm::vec3 rotation = glm::degrees(component.Rotation);

				ImGuiUtil::DrawFloat3Control("Translation", component.Translation, 0.01f);
				ImGuiUtil::DrawFloat3Control("Rotation", rotation);
				ImGuiUtil::DrawFloat3Control("Scale", component.Scale, 0.01f);

				component.Rotation = glm::radians(rotation);
		});

		// Material Component
		ImGuiUtil::DrawComponent<MaterialComponent>("Material", entity, [](auto& component)
			{
				std::string material = component.Material->GetName();
				if (ImGuiUtil::DrawComboControl("Material", material, MaterialLibrary::GetNames()))
					component.Material = MaterialLibrary::Get(material);
			});


		ImGui::Separator();

		if (Button("Remove Entity", glm::vec2{ -1, 0 }, ImGuiUtil::ButtonType::Danger))
		{
			entity.Destroy();
			m_SelectionContext = Entity();
		}
	}

	void SceneHierarchyPanel::Save()
	{
		// TODO: implement...
		LOG_WARN("void SceneHierarchyPanel::Save() not implemented!");
	}

}
