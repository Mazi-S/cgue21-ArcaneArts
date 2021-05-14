#include "egpch.h"
#include "SceneHierarchyPanel.h"

#include <imgui.h>
#include "ImGuiUtil.h"

#include <entt.hpp>

#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Mesh.h"

#include "Engine/Renderer/MaterialLibrary.h"
#include "Engine/Renderer/MeshLibrary.h"
#include "Engine/Audio/SoundLibrary.h"

#include "Engine/Scene/Components.h"

#include "Engine/Scene/Systems/Util.h"

namespace Engine {

	SceneHierarchyPanel::SceneHierarchyPanel(CallbackFn newCallback, CallbackFn openCallback, CallbackFn saveCallback)
		: m_NewCallback(newCallback), m_OpenCallback(openCallback), m_SaveCallback(saveCallback)
	{ }

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = Entity();
	}

	void SceneHierarchyPanel::OnImGui()
	{
		ImGui::Begin("Scene Hierarchy", &m_Active, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New")) { m_NewCallback(); }
				if (ImGui::MenuItem("Open...")) { m_OpenCallback(); }
				if (ImGui::MenuItem("Save...")) { m_SaveCallback(); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (m_Context == nullptr)
		{
			ImGui::End();
			return;
		}

		ImGuiUtil::HeaderText("Scene");
		std::string mainCamera = m_Context->m_MainCamera != entt::null 
			? m_Context->m_Registry.get<Component::Core::TagComponent>(m_Context->m_MainCamera).Tag 
			: std::string();

		std::vector<std::string> cameraNames;
		std::map<std::string, Entity> cameras;

		auto view = m_Context->m_Registry.view<Component::Renderer::CameraComponent, Component::Core::TagComponent>();
		for (const entt::entity e : view)
		{
			auto& tagComp = view.get<Component::Core::TagComponent>(e);
			cameraNames.push_back(tagComp.Tag);
			cameras[tagComp.Tag] = Entity{ e, &m_Context->m_Registry };
		}

		if (ImGuiUtil::DrawComboControl("Main Camera", mainCamera, cameraNames))
			m_Context->SetMainCamera(cameras[mainCamera]);


		ImGui::Dummy({ 0,.5 });
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
				m_SelectionContext = m_Context->CreateEntity();
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
		using Identifier			= Engine::Component::Core::Identifier;
		using Unserializable		= Engine::Component::Core::Unserializable;
		using TagComponent			= Engine::Component::Core::TagComponent;
		using TransformComponent	= Engine::Component::Core::TransformComponent;
		using ParentComponent		= Engine::Component::Core::ParentComponent;

		using MeshComponent				= Engine::Component::Renderer::MeshComponent;
		using MaterialComponent			= Engine::Component::Renderer::MaterialComponent;
		using ShadowComponent			= Engine::Component::Renderer::ShadowComponent;
		using DirectionalLightComponent	= Engine::Component::Renderer::DirectionalLightComponent;
		using PointLightComponent		= Engine::Component::Renderer::PointLightComponent;
		using CameraComponent			= Engine::Component::Renderer::CameraComponent;

		using StaticColliderComponent		= Engine::Component::Physics::StaticColliderComponent;
		using CharacterControllerComponent	= Engine::Component::Physics::CharacterControllerComponent;
		using RigidComponent				= Engine::Component::Physics::RigidComponent;
		using RigidDynamicComponent			= Engine::Component::Physics::RigidDynamicComponent;
		using KinematicMovementComponent	= Engine::Component::Physics::KinematicMovementComponent;

		using Sound2DComponent		= Engine::Component::Audio::Sound2DComponent;
		using Sound3DComponent		= Engine::Component::Audio::Sound3DComponent;
		using ListenerComponent		= Engine::Component::Audio::ListenerComponent;

		ImGuiUtil::Text("ID", std::to_string(entity.GetID()));

		bool serializable = !entity.HasComponent<Unserializable>();
		if(ImGuiUtil::Checkbox("Serializable", serializable))
		{
			if (serializable)
				entity.RemoveComponent<Unserializable>();
			else
				entity.AddComponent<Unserializable>();
		}

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
		if (!entity.HasComponent<ParentComponent>())
			components.push_back("Parent");
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
		if (!entity.HasComponent<StaticColliderComponent>())
			components.push_back("StaticCollider");
		if (!entity.HasComponent<CharacterControllerComponent>())
			components.push_back("CharacterController");
		if (!entity.HasComponent<Sound2DComponent>())
			components.push_back("Sound2D");
		if (!entity.HasComponent<Sound3DComponent>())
			components.push_back("Sound3D");
		if (!entity.HasComponent<ListenerComponent>())
			components.push_back("AudioListener");

		if (components.size() > 0 && ImGuiUtil::DrawComboControl("Add Component", component, components))
		{
			if (component == "Parent")
				entity.AddComponent<ParentComponent>();
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
			if (component == "StaticCollider")
				entity.AddComponent<StaticColliderComponent>();
			if (component == "CharacterController")
				entity.AddComponent<CharacterControllerComponent>();
			if (component == "Sound2D")
				entity.AddComponent<Sound2DComponent>();
			if (component == "Sound3D")
				entity.AddComponent<Sound3DComponent>();
			if (component == "AudioListener")
				entity.AddComponent<ListenerComponent>();
		}

		ImGui::Separator();

		// Parent Component
		ImGuiUtil::DrawComponent<ParentComponent>("Parent", entity, [](entt::registry* registryHandle, entt::entity entityHandle)
			{
				Entity entity = {entityHandle, registryHandle};
				Entity parent = entity.GetParent();
				
				auto view = registryHandle->view<Identifier, TagComponent>(entt::exclude<Component::Core::Unserializable>);
				std::map<std::string, entt::entity> entities;
				std::vector<std::string> names;
				for (auto e : view)
				{
					auto& [id, tagComp] = view.get<Identifier, TagComponent>(e);
					std::string name = tagComp.Tag + " (" + std::to_string(id.ID) + ")";
					entities[name] = e;
					names.push_back(name);
				}

				std::string parentName = parent ? parent.GetComponent<TagComponent>().Tag + " (" + std::to_string(parent.GetID()) + ")" : "none";
				if (ImGuiUtil::DrawComboControl("Parent", parentName, names))
					entity.GetComponent<ParentComponent>().Parent = entities[parentName];

				ImGuiUtil::Text("Description", "Specifies if the entity has a parent entity.");
			}, &m_Context->m_Registry, entity);

		// Transform Component
		ImGuiUtil::DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				glm::vec3 rotation = glm::degrees(component.Rotation);

				ImGuiUtil::DrawFloat3Control("Translation", component.Translation, 0, 0, 0.001f);
				if (ImGuiUtil::DrawFloat3Control("Rotation", rotation))
					component.Rotation = glm::radians(rotation);
				ImGuiUtil::DrawFloat3Control("Scale", component.Scale, 0.01f);
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

		// Static Collider Component
		ImGuiUtil::DrawComponent<StaticColliderComponent>("Static Collider", entity, [](auto& component)
			{
				ImGuiUtil::Text("Description", "Specifies if the entity has a physical collider.");
				ImGui::Dummy({ 0, .5 });
				ImGuiUtil::Text("Note", "Collider must be created after the TransformComponent and\nthe MeshComponent. Updates on these components will not\nbe reflected automatically to the collider.");
		}, true);

		// Character Controller Component
		ImGuiUtil::DrawComponent<CharacterControllerComponent>("Character Controller", entity, [](auto& component)
			{
				int active = component.Active;
				if (ImGuiUtil::DrawComboControl("Active", active, { "false", "true" }))
					component.Active = active;

				ImGuiUtil::DrawFloatControl("TranslationSpeed", component.TranslationSpeed, 0, 20, 0.01);
				
				float rotationSpeed = component.RotationSpeed * 1000.0f;
				if (ImGuiUtil::DrawFloatControl("RotationSpeed", rotationSpeed, 1, 10, 0.01))
					component.RotationSpeed = rotationSpeed / 1000.0f;

				ImGui::Dummy({ 0, .5 });
				ImGuiUtil::DrawFloatControl("Standing Height", component.StandingHeight, 0, 10, 0.01);
				ImGuiUtil::DrawFloatControl("Crouching Height", component.CrouchingHeight, 0, 10, 0.01);
				ImGuiUtil::DrawFloatControl("Radius", component.Radius, 0, 10, 0.01);

				ImGui::Dummy({ 0, .5 });

				ImGuiUtil::Text("Mouse", std::to_string((int)component.MouseX) + ", " + std::to_string((int)component.MouseY));
				ImGuiUtil::Text("Jump", std::to_string(component.Jump));
				ImGuiUtil::Text("Crouching", component.Crouching ? "true" : "false");
		}, true);

		// Rigid Actor
		ImGuiUtil::DrawComponent<RigidComponent>("Rigid Actor", entity, [](auto& component)
			{
				ImGui::Dummy({ 0, .5 });
				ImGuiUtil::Text("Description", "Specifies the physical representation.");
		});

		// Rigid Dynamic Actor
		ImGuiUtil::DrawComponent<RigidDynamicComponent>("Rigid Dynamic Actor", entity, [](auto& component)
			{
				ImGuiUtil::Text("actor", component.Actor == nullptr ? "nullptr" : "valid");
				ImGui::Dummy({ 0, .5 });
				ImGuiUtil::Text("Description", "Specifies the dynamic physical representation.");
		});

		// Kinematic Movement
		ImGuiUtil::DrawComponent<KinematicMovementComponent>("Kinematic Movement", entity, [](auto& component)
			{
				ImGuiUtil::DrawFloat3Control("Movement/s", component.Movement, 0, 0, 0.01);

				glm::vec3 euler = glm::eulerAngles(component.Rotation);
				if (ImGuiUtil::DrawFloat3Control("Rotation", euler, 0, 20, 0.01))
					component.Rotation = glm::quat(euler);
				ImGui::Dummy({ 0, .5 });
				ImGuiUtil::Text("Description", "Specifies the movement of a kinematic actor.");
		});

		// Sound 2D
		ImGuiUtil::DrawComponent<Sound2DComponent>("Sound [2D]", entity, [](Entity& entity)
			{
				Sound2DComponent& component = entity.GetComponent<Sound2DComponent>();

				bool update = false;

				int loop = component.Loop;
				if (ImGuiUtil::DrawComboControl("Active", loop, { "false", "true" }))
				{
					component.Loop = loop;
					update = true;
				}

				if (ImGuiUtil::DrawFloatControl("Volume", component.Volume, 0.0f, 1.0f, 0.001))
				{
					if (component.Sound != nullptr)
						component.Sound->setVolume(component.Volume);
				}


				std::string soundSource = component.SoundSource;
				if (ImGuiUtil::DrawComboControl("SoundSource", soundSource, SoundLibrary::GetNames()))
				{
					component.SoundSource = soundSource;
					component.Volume = SoundLibrary::Get(soundSource)->GetVolume();
					update = true;
				}

				if (update)
					entity.Update<Sound2DComponent>();

			}, entity);

		// Sound 3D
		ImGuiUtil::DrawComponent<Sound3DComponent>("Sound [3D]", entity, [](Entity& entity)
			{
				Sound3DComponent& component = entity.GetComponent<Sound3DComponent>();

				bool update = false;

				int loop = component.Loop;
				if (ImGuiUtil::DrawComboControl("Active", loop, { "false", "true" }))
				{
					component.Loop = loop;
					update = true;
				}

				if (ImGuiUtil::DrawFloatControl("Volume", component.Volume, 0.0f, 1.0f, 0.001))
				{
					if (component.Sound != nullptr)
						component.Sound->setVolume(component.Volume);
				}

				std::string soundSource = component.SoundSource;
				if (ImGuiUtil::DrawComboControl("SoundSource", soundSource, SoundLibrary::GetNames()))
				{
					component.SoundSource = soundSource;
					component.Volume = SoundLibrary::Get(soundSource)->GetVolume();
					update = true;
				}

				if (update)
					entity.Update<Sound3DComponent>();

			}, entity);

		// Listener Component
		ImGuiUtil::DrawComponent<ListenerComponent>("Audio Listener", entity, [](auto& component)
			{
				ImGuiUtil::Text("Description", "Specifies if the entity is the audio listener.");
		});


		ImGui::Separator();

		if (Button("Remove Entity", glm::vec2{ -1, 0 }, ImGuiUtil::ButtonType::Danger))
		{
			entity.Destroy();
			m_SelectionContext = Entity();
		}
	}

}
