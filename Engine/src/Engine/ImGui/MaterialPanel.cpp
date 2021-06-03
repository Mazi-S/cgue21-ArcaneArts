#include "egpch.h"
#include "MaterialPanel.h"
#include "Engine/Renderer/MaterialLibrary.h"
#include "Engine/Renderer/ShaderLibrary.h"
#include "Engine/Renderer/TextureLibrary.h"

#include <imgui.h>
#include "ImGuiUtil.h"
#include <windows.h>
#include <fileapi.h>

namespace Engine {

	void MaterialPanel::OnImGui()
	{
		ImGui::Begin("Material Library", &m_Active, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save")) { Save(); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGuiUtil::HeaderText("MaterialLibrary");

		for (auto entry : MaterialLibrary::s_Materials)
		{
			Ref<Material> material = entry.second;
			DrawMaterialNode(material);
		}

		ImGui::End();

		ImGui::Begin("Material");

		if (m_SelectionContext != nullptr)
		{
			DrawMaterial(m_SelectionContext);
		}
		else
		{
			if (ImGuiUtil::Button("Add Material", glm::vec2{ -1, 0 }, ImGuiUtil::ButtonType::Success))
			{
				m_SelectionContext = MaterialLibrary::Create("New Material");
			}
		}

		ImGui::End();
	}

	void MaterialPanel::DrawMaterialNode(Ref<Material> material)
	{
		auto& name = material->GetName();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= (m_SelectionContext == material) ? ImGuiTreeNodeFlags_Selected : 0;

		bool opened = ImGui::TreeNodeEx(material.get(), flags, name.c_str());
		if (ImGui::IsItemClicked())
			m_SelectionContext = m_SelectionContext != material ? material : nullptr;

		if (opened)
			ImGui::TreePop();
	}

	void MaterialPanel::DrawMaterial(Ref<Material> material)
	{
		// Name (ID)
		std::string name = material->GetName();
		if (ImGuiUtil::InputText("Name", name))
			MaterialLibrary::Rename(material->GetName(), name);

		// Shader
		std::string shader = material->m_Shader;
		if (ImGuiUtil::DrawComboControl("Shader", shader, ShaderLibrary::GetNames()))
			material->SetShader(shader);

		// Textures
		if (material->GetTextures().size() > 0)
		{
			for (auto& entry : material->GetTextures())
			{
				std::stringstream ss;
				ss << "Texture " << entry.first;
				std::string texture = entry.second;
				if (ImGuiUtil::DrawComboControl(ss.str(), texture, Texture2DLibrary::GetNames()))
					material->SetTexture(entry.first, texture);
			}
		}

		bool add = false;
		bool remove = false;
		if (ImGuiUtil::Button("", "Add Texture", add, ImGuiUtil::ButtonType::Default, "Remove Texture", remove, ImGuiUtil::ButtonType::Default))
		{
			if (add) material->SetTexture(material->GetTextures().size());
			else if (remove) material->RemoveTexture(material->GetTextures().size() - 1);
		}

		ImGui::NewLine();

		glm::vec3 ambient = material->GetAmbient();
		glm::vec3 diffuse = material->GetDiffuse();
		glm::vec3 specular = material->GetSpecular();
		float shininess = material->GetShininess();
		if (ImGuiUtil::DrawFloat3Control("Ambient", ambient, 0, 1, 0.001))
			material->SetAmbient(ambient);

		if (ImGuiUtil::DrawFloat3Control("Diffuse", diffuse, 0, 1, 0.001))
			material->SetDiffuse(diffuse);

		if (ImGuiUtil::DrawFloat3Control("Specular", specular, 0, 1, 0.001))
			material->SetSpecular(specular);

		if (ImGuiUtil::DrawFloatControl("Shininess", shininess, 0.1f, 50))
			material->SetShininess(shininess);

		ImGui::NewLine();

		if (ImGuiUtil::Button("Remove Material", glm::vec2{ -1, 0 }, ImGuiUtil::ButtonType::Danger))
		{
			MaterialLibrary::Remove(material->GetName());
			m_SelectionContext = nullptr;
		}
	}

	void MaterialPanel::Save()
	{
		MaterialLibrary::Save();
	}

}

