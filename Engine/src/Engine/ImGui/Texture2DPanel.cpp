#include "egpch.h"
#include "Texture2DPanel.h"
#include "Engine/Renderer/TextureLibrary.h"

#include <imgui.h>
#include "ImGuiUtil.h"
#include "Engine/Util/OpenGLEnumConverter.h"

namespace Engine {

	void Texture2DPanel::OnImGui()
	{
		ImGui::Begin("Texture 2D", &m_Active, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save")) { /* Do stuff */ }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGuiUtil::HeaderText("TextureLibrary (2D)");

		for (auto entry : Texture2DLibrary::s_Textures2D)
		{
			Ref<OpenGL::GlTexture2D> texture = entry.second;
			DrawTextureNode(texture);
		}

		if (m_SelectionContext != nullptr)
		{
			ImGui::Separator();
			ImGuiUtil::HeaderText("Texture");
			DrawTexture(m_SelectionContext);
		}

		ImGui::End();
	}

	void Texture2DPanel::DrawTextureNode(Ref<OpenGL::GlTexture2D>& texture)
	{
		auto& name = texture->GetName();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= (m_SelectionContext == texture) ? ImGuiTreeNodeFlags_Selected : 0;

		bool opened = ImGui::TreeNodeEx(texture.get(), flags, name.c_str());
		if (ImGui::IsItemClicked())
			m_SelectionContext = texture;

		if (opened)
			ImGui::TreePop();
	}

	void Texture2DPanel::DrawTexture(Ref<OpenGL::GlTexture2D>& texture)
	{
		ImGuiUtil::Text("Name", texture->GetName());
		if (!texture->IsDynamic())
			ImGuiUtil::Text("Path", texture->GetPath());
		ImGui::NewLine();

		// Specification
		OpenGL::Texture2DSpecification spec = texture->GetSpecification();

		ImGuiUtil::Text("Size", std::to_string(spec.Width) + " x " + std::to_string(spec.Height));

		ImGuiUtil::Text("Internalformat", Util::GlEnumConverter::ToString(spec.Internalformat));
		ImGuiUtil::Text("Format", Util::GlEnumConverter::ToString(spec.Format));
		ImGuiUtil::Text("Type", Util::GlEnumConverter::ToString(spec.Type));

		// Texture Wrapping
		ImGuiUtil::Text("Wrap_S", Util::GlEnumConverter::ToString(spec.Wrap_T));
		ImGuiUtil::Text("Wrap_T", Util::GlEnumConverter::ToString(spec.Wrap_T));
		
		std::stringstream ss;
		ss << "[" << spec.Border.x << ", " << spec.Border.y << ", " << spec.Border.z << ", " << spec.Border.w <<"]";
		ImGuiUtil::Text("Border", ss.str());

		ImGuiUtil::Text("Min Filter", Util::GlEnumConverter::ToString(spec.Min_Filter));
		ImGuiUtil::Text("Mag Filter", Util::GlEnumConverter::ToString(spec.Mag_Filter));

		ImGuiUtil::Text("ComparisonMode", Util::GlEnumConverter::ToString(spec.ComparisonMode));
		ImGuiUtil::Text("Mipmaps", spec.Mipmaps ? "true" : "false");

		ImGui::NewLine();
		glm::vec2 size = { texture->GetSpecification().Width, texture->GetSpecification().Height };
		size *= ImGui::GetContentRegionAvail().x / texture->GetSpecification().Width;
		ImGui::Image((void*)(intptr_t)texture->m_TextureID, { size.x, size.y }, {0, 1}, {1, 0});
	}

}

