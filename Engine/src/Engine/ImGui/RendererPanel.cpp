#include "egpch.h"
#include "RendererPanel.h"

#include <imgui.h>
#include "ImGuiUtil.h"

namespace Engine {

	void RendererPanel::OnImGui()
	{
		ImGui::Begin("Renderer", &m_Active, ImGuiWindowFlags_NoCollapse);

		ImGuiUtil::HeaderText("Renderer");

		int hdr = Renderer::s_HDR;
		if (ImGuiUtil::DrawComboControl("HDR", hdr, { "false", "true" }))
			Renderer::s_HDR = hdr;

		ImGuiUtil::DrawFloatControl("Exposure", Renderer::s_Exposure, 0.01, 20, 0.01);

		int bloom = Renderer::s_Bloom;
		if (ImGuiUtil::DrawComboControl("Bloom", bloom, { "false", "true" }))
			Renderer::s_Bloom = bloom;

		int gamma = Renderer::s_GammaCorrection;
		if (ImGuiUtil::DrawComboControl("Gamma Correction", gamma, { "false", "true" }))
			Renderer::s_GammaCorrection = gamma;

		ImGui::End();
	}

}
