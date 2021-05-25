#include "egpch.h"
#include "Texture2DPanel.h"
#include "Engine/Renderer/TextureLibrary.h"

#include <imgui.h>
#include <glad/glad.h>

#include "ImGuiUtil.h"
#include "Engine/Util/OpenGLEnumConverter.h"

namespace Engine {

	void Texture2DPanel::OnImGui()
	{
		ImGui::Begin("Texture Library (2D)", &m_Active, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save")) { Save(); }
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGuiUtil::HeaderText("Texture Library (2D)");

		for (auto entry : Texture2DLibrary::s_Textures2D)
		{
			Ref<OpenGL::GlTexture2D> texture = entry.second;
			DrawTextureNode(texture);
		}

		ImGui::End();

		ImGui::Begin("Texture");

		if (m_SelectionContext != nullptr)
		{
			ImGuiUtil::HeaderText("Texture");
			DrawTexture(m_SelectionContext);
		}
		else
		{
			if (ImGuiUtil::Button("Add Texture", glm::vec2{ -1, 0 }, ImGuiUtil::ButtonType::Success))
			{
				m_SelectionContext = Texture2DLibrary::Create("New Texture", "assets/textures/WoodFloor.jpg");
			}
		}

		ImGui::End();
	}

	void Texture2DPanel::DrawTextureNode(Ref<OpenGL::GlTexture2D> texture)
	{
		auto& name = texture->GetName();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
		flags |= (m_SelectionContext == texture) ? ImGuiTreeNodeFlags_Selected : 0;

		bool opened = ImGui::TreeNodeEx(texture.get(), flags, name.c_str());
		if (ImGui::IsItemClicked())
			m_SelectionContext = m_SelectionContext != texture ? texture : nullptr;

		if (opened)
			ImGui::TreePop();
	}

	void Texture2DPanel::DrawTexture(Ref<OpenGL::GlTexture2D> texture)
	{
		if (texture->IsSystem())
		{
			// Name (ID)
			ImGuiUtil::Text("Name", texture->GetName());
		}
		else
		{
			// Name (ID)
			std::string name = texture->GetName();
			if (ImGuiUtil::InputText("Name", name))
				Texture2DLibrary::Rename(texture->GetName(), name);

			// Path
			std::string path = texture->GetPath();
			if (ImGuiUtil::InputPath("Path", path, "PNG (*.png)\0*.png\0"))
			{
				texture->m_Path = path;
				texture->Reload();
			}
		}

		ImGui::Dummy({ 0, 2 });

		// Specification
		OpenGL::Texture2DSpecification spec = texture->GetSpecification();

		ImGuiUtil::Text("Size", std::to_string(spec.Width) + " x " + std::to_string(spec.Height));

		ImGuiUtil::Text("Internalformat", Util::GlEnumConverter::ToString(spec.Internalformat));
		ImGuiUtil::Text("Format", Util::GlEnumConverter::ToString(spec.Format));
		ImGuiUtil::Text("Type", Util::GlEnumConverter::ToString(spec.Type));

		ImGui::Dummy({ 0, 2 });

		if (texture->IsSystem())
		{
			ImGuiUtil::Text("Wrap_S", Util::GlEnumConverter::ToString(spec.Wrap_T));
			ImGuiUtil::Text("Wrap_T", Util::GlEnumConverter::ToString(spec.Wrap_T));
		
			std::stringstream ss;
			ss << "[" << spec.Border.x << ", " << spec.Border.y << ", " << spec.Border.z << ", " << spec.Border.w <<"]";
			ImGuiUtil::Text("Border", ss.str());

			ImGuiUtil::Text("Min Filter", Util::GlEnumConverter::ToString(spec.Min_Filter));
			ImGuiUtil::Text("Mag Filter", Util::GlEnumConverter::ToString(spec.Mag_Filter));

			ImGuiUtil::Text("ComparisonMode", Util::GlEnumConverter::ToString(spec.ComparisonMode));
			ImGuiUtil::Text("Mipmaps", spec.Mipmaps ? "true" : "false");
		}
		else
		{
			// Wrap
			std::vector<std::string> wrap;
			wrap.push_back("GL_REPEAT");
			wrap.push_back("GL_MIRRORED_REPEAT");
			wrap.push_back("GL_CLAMP_TO_EDGE");
			wrap.push_back("GL_CLAMP_TO_BORDER");

			std::string wrap_s = Util::GlEnumConverter::ToString(spec.Wrap_S);
			std::string wrap_t = Util::GlEnumConverter::ToString(spec.Wrap_T);

			if (ImGuiUtil::DrawComboControl("Wrap_S", wrap_s, wrap))
			{
				if (wrap_s == "GL_REPEAT")
					texture->m_Specification.Wrap_S = GL_REPEAT;
				else if (wrap_s == "GL_MIRRORED_REPEAT")
					texture->m_Specification.Wrap_S = GL_MIRRORED_REPEAT;
				else if (wrap_s == "GL_CLAMP_TO_EDGE")
					texture->m_Specification.Wrap_S = GL_CLAMP_TO_EDGE;
				else if (wrap_s == "GL_CLAMP_TO_BORDER")
					texture->m_Specification.Wrap_S = GL_CLAMP_TO_BORDER;

				texture->Reload();
			}

			if (ImGuiUtil::DrawComboControl("Wrap_T", wrap_t, wrap))
			{
				if (wrap_t == "GL_REPEAT")
					texture->m_Specification.Wrap_T = GL_REPEAT;
				else if (wrap_t == "GL_MIRRORED_REPEAT")
					texture->m_Specification.Wrap_T = GL_MIRRORED_REPEAT;
				else if (wrap_t == "GL_CLAMP_TO_EDGE")
					texture->m_Specification.Wrap_T = GL_CLAMP_TO_EDGE;
				else if (wrap_t == "GL_CLAMP_TO_BORDER")
					texture->m_Specification.Wrap_T = GL_CLAMP_TO_BORDER;

				texture->Reload();
			}

			// Filter
			std::vector<std::string> filter;
			filter.push_back("GL_NEAREST");
			filter.push_back("GL_LINEAR");
			filter.push_back("GL_NEAREST_MIPMAP_NEAREST");
			filter.push_back("GL_LINEAR_MIPMAP_NEAREST");
			filter.push_back("GL_NEAREST_MIPMAP_LINEAR");
			filter.push_back("GL_LINEAR_MIPMAP_LINEAR");

			std::string min_filter = Util::GlEnumConverter::ToString(spec.Min_Filter);
			std::string mag_filter = Util::GlEnumConverter::ToString(spec.Mag_Filter);

			if (ImGuiUtil::DrawComboControl("Min Filter", min_filter, filter))
			{
				if (min_filter == "GL_NEAREST")
					texture->m_Specification.Min_Filter = GL_NEAREST;
				else if (min_filter == "GL_LINEAR")
					texture->m_Specification.Min_Filter = GL_LINEAR;
				else if (min_filter == "GL_NEAREST_MIPMAP_NEAREST")
					texture->m_Specification.Min_Filter = GL_NEAREST_MIPMAP_NEAREST;
				else if (min_filter == "GL_LINEAR_MIPMAP_NEAREST")
					texture->m_Specification.Min_Filter = GL_LINEAR_MIPMAP_NEAREST;
				else if (min_filter == "GL_NEAREST_MIPMAP_LINEAR")
					texture->m_Specification.Min_Filter = GL_NEAREST_MIPMAP_LINEAR;
				else if (min_filter == "GL_LINEAR_MIPMAP_LINEAR")
					texture->m_Specification.Min_Filter = GL_LINEAR_MIPMAP_LINEAR;

				texture->Reload();
			}

			if (ImGuiUtil::DrawComboControl("Mag Filter", mag_filter, filter))
			{
				if (mag_filter == "GL_NEAREST")
					texture->m_Specification.Mag_Filter = GL_NEAREST;
				else if (mag_filter == "GL_LINEAR")
					texture->m_Specification.Mag_Filter = GL_LINEAR;
				else if (mag_filter == "GL_NEAREST_MIPMAP_NEAREST")
					texture->m_Specification.Mag_Filter = GL_NEAREST_MIPMAP_NEAREST;
				else if (mag_filter == "GL_LINEAR_MIPMAP_NEAREST")
					texture->m_Specification.Mag_Filter = GL_LINEAR_MIPMAP_NEAREST;
				else if (mag_filter == "GL_NEAREST_MIPMAP_LINEAR")
					texture->m_Specification.Mag_Filter = GL_NEAREST_MIPMAP_LINEAR;
				else if (mag_filter == "GL_LINEAR_MIPMAP_LINEAR")
					texture->m_Specification.Mag_Filter = GL_LINEAR_MIPMAP_LINEAR;

				texture->Reload();
			}

			int mipmaps = spec.Mipmaps;
			if (ImGuiUtil::DrawComboControl("Mipmaps", mipmaps, { "false", "true" }))
			{
				texture->m_Specification.Mipmaps = mipmaps;
				texture->Reload();
			}
		}

		ImGui::Dummy({ 0, 2 });

		glm::vec2 size = { texture->GetSpecification().Width, texture->GetSpecification().Height };
		size *= ImGui::GetContentRegionAvail().x / texture->GetSpecification().Width;
		ImGui::Image((void*)(intptr_t)texture->m_TextureID, { size.x, size.y }, { 0, 1 }, { 1, 0 });
	}

	void Texture2DPanel::Save()
	{
		Texture2DLibrary::Save();
	}

}

