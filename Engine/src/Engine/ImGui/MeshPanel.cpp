#include "egpch.h"
#include "MeshPanel.h"
#include "Engine/Renderer/MeshLibrary.h"

#include <imgui.h>
#include "ImGuiUtil.h"

namespace Engine {

	void Engine::MeshPanel::OnImGui()
	{
		ImGui::Begin("Mesh Library", &m_Active, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save")) { Save(); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGuiUtil::HeaderText("Mesh Library");

		for (auto entry : MeshLibrary::s_Meshes)
		{
			Ref<Mesh> shader = entry.second;
			DrawMeshNode(shader);
		}

		ImGui::End();

		ImGui::Begin("Mesh");

		if (m_SelectionContext != nullptr)
		{
			DrawMesh(m_SelectionContext);
		}
		else
		{
			if (ImGuiUtil::Button("Add Mesh", glm::vec2{ -1, 0 }, ImGuiUtil::ButtonType::Success))
			{
				m_SelectionContext = MeshLibrary::Create("New Mesh", "assets/objects/cube.obj");
			}
		}

		ImGui::End();
	}

	void MeshPanel::DrawMeshNode(Ref<Mesh>& mesh)
	{
		auto& name = mesh->GetName();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= (m_SelectionContext == mesh) ? ImGuiTreeNodeFlags_Selected : 0;

		bool opened = ImGui::TreeNodeEx(mesh.get(), flags, name.c_str());
		if (ImGui::IsItemClicked())
			m_SelectionContext = m_SelectionContext != mesh ? mesh : nullptr;

		if (opened)
			ImGui::TreePop();
	}

	void MeshPanel::DrawMesh(Ref<Mesh>& mesh)
	{
		// Name (ID)
		std::string name = mesh->GetName();
		if (ImGuiUtil::InputText("Name", name))
			MeshLibrary::Rename(mesh->GetName(), name);

		// Path
		std::string path = mesh->GetPath();
		if (ImGuiUtil::InputPath("Path", path, "Object File (*.obj)\0*.obj\0"))
		{
			mesh->m_Path = path;
			mesh->Reload();
		}

		// Tangent space
		if (ImGuiUtil::Checkbox("Tangent space", mesh->m_TangentSpace))
		{
			mesh->Reload();
		}

		ImGui::Separator();

		ImGuiUtil::Text("Vertices", std::to_string(mesh->m_Positions.size()));
		ImGuiUtil::Text("Submeshes", std::to_string(mesh->m_Submeshes.size()));
		
		ImGui::Separator();
		
		if (ImGuiUtil::Button("Remove Mesh", glm::vec2{ -1, 0 }, ImGuiUtil::ButtonType::Danger))
		{
			MeshLibrary::Remove(mesh->GetName());
			m_SelectionContext = nullptr;
		}
	}

	void MeshPanel::Save()
	{
		MeshLibrary::Save();
	}

}