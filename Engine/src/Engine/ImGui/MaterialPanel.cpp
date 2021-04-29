#include "egpch.h"
#include "MaterialPanel.h"
#include "ImGuiUtil.h"

#include <imgui.h>

namespace Engine {

	void MaterialPanel::OnImGui()
	{
		ImGui::Begin("Materials");

		for (auto entry : MaterialLibrary::s_Materials)
		{
			Ref<Material> material = entry.second;
			DrawMaterialNode(material);
		}

		if (m_SelectionContext != nullptr)
		{
			ImGui::Separator();
			DrawMaterial(m_SelectionContext);
		}

		ImGui::End();
	}

	void MaterialPanel::DrawMaterialNode(Ref<Material>& material)
	{
		auto& name = material->GetName();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= (m_SelectionContext == material) ? ImGuiTreeNodeFlags_Selected : 0;

		bool opened = ImGui::TreeNodeEx(material.get(), flags, name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = material;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void MaterialPanel::DrawMaterial(Ref<Material>& material)
	{
		ImGuiUtil::Text("Name", material->GetName());
		ImGuiUtil::Text("Shader", material->m_Shader->GetName());

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

		if (ImGuiUtil::DrawFloatControl("Shininess", shininess, 1, 50))
			material->SetShininess(shininess);
	}


}

