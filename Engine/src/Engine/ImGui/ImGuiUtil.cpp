#include "egpch.h"
#include "ImGuiUtil.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

bool Engine::ImGuiUtil::DrawFloatControl(const std::string& label, float& value, float min, float max, float speed, float columnWidth)
{
	ImGui::PushID(label.c_str());
	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(0);
	bool update = ImGui::DragFloat("##DragFloat", &value, speed, min, max, "%.2f");
	ImGui::PopID();
	ImGui::Columns(1);
	return update;
}

bool Engine::ImGuiUtil::DrawFloat3Control(const std::string& label, glm::vec3& values, float min, float max, float speed, float columnWidth)
{
	ImGui::PushID(label.c_str());
	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(0);
	bool update = ImGui::DragFloat3("##DragFloat3", glm::value_ptr(values), speed, min, max, "%.2f");
	ImGui::PopID();
	ImGui::Columns(1);
	return update;
}

bool Engine::ImGuiUtil::DrawColorControl(const std::string& label, glm::vec4& values, float columnWidth)
{
	ImGui::PushID(label.c_str());
	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(0);
	bool update = ImGui::ColorEdit4("##ColorEdit4", glm::value_ptr(values), ImGuiColorEditFlags__OptionsDefault);
	ImGui::PopID();
	ImGui::Columns(1);
	return update;
}

bool Engine::ImGuiUtil::DrawColorControl(const std::string& label, glm::vec3& values, float columnWidth)
{
	ImGui::PushID(label.c_str());
	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(0);
	bool update = ImGui::ColorEdit3("##ColorEdit3", glm::value_ptr(values), ImGuiColorEditFlags__OptionsDefault);
	ImGui::PopID();
	ImGui::Columns(1);
	return update;
}

void Engine::ImGuiUtil::Text(const std::string& label, const std::string& text, float columnWidth)
{
	ImGui::PushID(label.c_str());
	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(0);
	ImGui::Text(text.c_str());
	ImGui::PopID();
	ImGui::Columns(1);
}

void Engine::ImGuiUtil::HeaderText(const std::string& text)
{
}

void Engine::ImGuiUtil::BoldText(const std::string& text)
{
}
