#include "egpch.h"
#include "ShaderPanel.h"
#include "Engine/Renderer/ShaderLibrary.h"

#include <imgui.h>
#include "ImGuiUtil.h"

namespace Engine {

	void ShaderPanel::OnImGui()
	{
		ImGui::Begin("Shaders");

		for (auto entry : ShaderLibrary::s_Shaders)
		{
			Ref<OpenGL::GlShader> shader = entry.second;
			DrawShaderNode(shader);
		}

		if (m_SelectionContext != nullptr)
		{
			ImGui::Separator();
			DrawShader(m_SelectionContext);
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
		ImGuiUtil::Text("Name", shader->GetName());
		ImGuiUtil::Text("Path", shader->GetPath());
	}
}

