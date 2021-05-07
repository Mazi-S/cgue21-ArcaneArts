#include "egpch.h"
#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include "ImGuiUtil.h"

#include <entt.hpp>

#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Mesh.h"

#include "Engine/Renderer/MaterialLibrary.h"
#include "Engine/Renderer/MeshLibrary.h"
#include "Engine/Scene/Components.h"

#include "Engine/Scene/Systems/Util.h"

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

		ImGuiUtil::HeaderText("Scene Hierarchy");

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
		using TagComponent				= Engine::Component::Core::TagComponent;
		using TransformComponent		= Engine::Component::Core::TransformComponent;
		using MeshComponent				= Engine::Component::Renderer::MeshComponent;
		using MaterialComponent			= Engine::Component::Renderer::MaterialComponent;
		using ShadowComponent			= Engine::Component::Renderer::ShadowComponent;
		using DirectionalLightComponent	= Engine::Component::Renderer::DirectionalLightComponent;
		using PointLightComponent		= Engine::Component::Renderer::PointLightComponent;
		using CameraComponent			= Engine::Component::Renderer::CameraComponent;

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
		if (!entity.HasComponent<MeshComponent>())
			components.push_back("Mesh");
		if (!entity.HasComponent<MaterialComponent>())
			components.push_back("Material");
		if (!entity.HasComponent<ShadowComponent>())
			components.push_back("Shadow");
		if (!entity.HasComponent<DirectionalLightComponent>())
			components.push_back("DirectionalLight");
		if (!entity.HasComponent<PointLightComponent>())
			components.push_back("PointLight");
		if (!entity.HasComponent<CameraComponent>())
			components.push_back("Camera");

		if (components.size() > 0 && ImGuiUtil::DrawComboControl("Add Component", component, components))
		{
			if (component == "Transform")
				entity.AddComponent<TransformComponent>();
			if (component == "Mesh")
				entity.AddComponent<MeshComponent>();
			if (component == "Material")
				entity.AddComponent<MaterialComponent>();
			if (component == "Shadow")
				entity.AddComponent<ShadowComponent>();
			if (component == "DirectionalLight")
				entity.AddComponent<DirectionalLightComponent>();
			if (component == "PointLight")
				entity.AddComponent<PointLightComponent>();
			if (component == "Camera")
				entity.AddComponent<CameraComponent>();
		}

		ImGui::Separator();

		// Transform Component
		ImGuiUtil::DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				glm::vec3 rotation = glm::degrees(component.Rotation);

				ImGuiUtil::DrawFloat3Control("Translation", component.Translation, 0.01f);
				if (ImGuiUtil::DrawFloat3Control("Rotation", rotation))
					component.Rotation = glm::radians(rotation);
				ImGuiUtil::DrawFloat3Control("Scale", component.Scale, 0.01f);

				component.Rotation = glm::radians(rotation);
		});

		// Material Component
		ImGuiUtil::DrawComponent<MeshComponent>("Mesh", entity, [](auto& component)
			{
				std::string mesh = component.Mesh;
				if (ImGuiUtil::DrawComboControl("Mesh", mesh, MeshLibrary::GetNames()))
					component.Mesh = mesh;
			});


		// Material Component
		ImGuiUtil::DrawComponent<MaterialComponent>("Material", entity, [](auto& component)
			{
				std::string material = component.Material;
				if (ImGuiUtil::DrawComboControl("Material", material, MaterialLibrary::GetNames()))
					component.Material = material;
		});

		// Shadow Component
		ImGuiUtil::DrawComponent<ShadowComponent>("Shadow", entity, [](auto& component)
			{
				ImGuiUtil::Text("Description", "Specifies if the entity casts a shadow.");
		});

		// Directional Light Component
		ImGuiUtil::DrawComponent<DirectionalLightComponent>("Directional Light", entity, [](auto& component)
			{
				ImGuiUtil::DrawFloat3Control("Direction", component.Direction, 0.01f);
				ImGuiUtil::DrawFloat3Control("Color", component.Color, 0.01f);
		});

		// Point Light Component
		ImGuiUtil::DrawComponent<PointLightComponent>("Point Light", entity, [](auto& component)
			{
				ImGuiUtil::DrawFloat3Control("Color", component.Color, 0.01f);
				ImGuiUtil::DrawFloatControl("Constant", component.Constant, 0, 10, 0.001f);
				ImGuiUtil::DrawFloatControl("Linear", component.Linear, 0, 10, 0.001f);
				ImGuiUtil::DrawFloatControl("Quadratic", component.Quadratic, 0, 10, 0.001f);
		});

		// Camera Component
		ImGuiUtil::DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
			{
				bool recalculate = false;

				float fov = glm::degrees(component.FOV);
				if (ImGuiUtil::DrawFloatControl("FOV", fov, 0, 360))
				{
					recalculate = true;
					component.FOV = glm::radians(fov);
				}
				if (ImGuiUtil::DrawFloatControl("Near", component.Near))
					recalculate = true;
				if (ImGuiUtil::DrawFloatControl("Far", component.Far))
					recalculate = true;

				if (recalculate)
					System::Util::RecalculateProjection(component);
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
