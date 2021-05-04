#include "egpch.h"
#include "ShaderPanel.h"
#include "Engine/Renderer/ShaderLibrary.h"

#include <imgui.h>
#include "ImGuiUtil.h"

namespace Engine {

	void ShaderPanel::OnImGui()
	{
		ImGui::Begin("Shaders", &m_Active, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save")) { Save(); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGuiUtil::HeaderText("ShaderLibrary");

		for (auto entry : ShaderLibrary::s_Shaders)
		{
			Ref<OpenGL::GlShader> shader = entry.second;
			DrawShaderNode(shader);
		}

		if (m_SelectionContext != nullptr)
		{
			ImGui::Separator();
			ImGuiUtil::HeaderText("Shader");
			DrawShader(m_SelectionContext);
		}
		else
		{
			if (ImGui::Button("Add Shader", { -1, 0 }))
			{
				m_SelectionContext = ShaderLibrary::Create("New Shader", "assets/shaders/DefaultShader.glsl");
			}
		}

		ImGui::End();
	}

	void ShaderPanel::DrawShaderNode(Ref<OpenGL::GlShader>& shader)
	{
		auto& name = shader->GetName();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= (m_SelectionContext == shader) ? ImGuiTreeNodeFlags_Selected : 0;

		bool opened = ImGui::TreeNodeEx(shader.get(), flags, name.c_str());
		if (ImGui::IsItemClicked())
			m_SelectionContext = shader;

		if (opened)
			ImGui::TreePop();
	}

	void ShaderPanel::DrawShader(Ref<OpenGL::GlShader>& shader)
	{
		// Name (ID)
		std::string name = shader->GetName();
		if (ImGuiUtil::InputText("Name", name))
			ShaderLibrary::Rename(shader->GetName(), name);

		// Path
		std::string path = shader->GetPath();
		if (ImGuiUtil::InputText("Path", path))
		{
			shader->m_Path = path;
			shader->Reload();
		}

		// Error
		if (shader->m_RendererID == 0)
		{
			ImGui::NewLine();
			ImGuiUtil::Text("Error", "Invalid shader!");
		}

		ImGui::NewLine();
		ImGui::NewLine();

		if (ImGui::Button("Remove Shader", { -1, 0 }))
		{
			ShaderLibrary::Remove(shader->GetName());
			m_SelectionContext = nullptr;
		}
	}


	void ShaderPanel::Save()
	{
		ShaderLibrary::Save();
	}
}

