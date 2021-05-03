#include "egpch.h"
#include "ImGuiUtil.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

bool Engine::ImGuiUtil::DrawComboControl(const std::string& label, int& selectionIndex, const std::vector<std::string> values, float columnWidth)
{
	ImGui::PushID(label.c_str());
	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(0);
	bool update = false;
	if (ImGui::BeginCombo("##BeginCombo", values[selectionIndex].c_str()))
	{
		for (int i = 0; i < values.size(); i++)
		{
			bool isSelected = i == selectionIndex;

			if (ImGui::Selectable(values[i].c_str(), isSelected))
			{
				selectionIndex = i;
				update = true;
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::Columns(1);
	ImGui::PopID();
	return update;
}

bool Engine::ImGuiUtil::DrawComboControl(const std::string& label, std::string& selection, const std::vector<std::string> values, float columnWidth)
{
	ImGui::PushID(label.c_str());
	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(0);
	bool update = false;
	if (ImGui::BeginCombo("##BeginCombo", selection.c_str()))
	{
		for (int i = 0; i < values.size(); i++)
		{
			bool isSelected = values[i] == selection;

			if (ImGui::Selectable(values[i].c_str(), isSelected))
			{
				selection = values[i];
				update = true;
			}

			if (isSelected)
				ImGui::SetItemDefaultFocus();
		}

		ImGui::EndCombo();
	}
	ImGui::Columns(1);
	ImGui::PopID();
	return update;
}

bool Engine::ImGuiUtil::DrawFloatControl(const std::string& label, float& value, float min, float max, float speed, float columnWidth)
{
	ImGui::PushID(label.c_str());
	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(0);
	bool update = ImGui::DragFloat("##DragFloat", &value, speed, min, max, "%.2f");
	ImGui::Columns(1);
	ImGui::PopID();
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
	ImGui::Columns(1);
	ImGui::PopID();
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
	ImGui::Columns(1);
	ImGui::PopID();
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
	ImGui::Columns(1);
	ImGui::PopID();
	return update;
}

bool Engine::ImGuiUtil::InputText(const std::string& label, std::string& text, float columnWidth)
{
	ImGui::PushID(label.c_str());
	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(0);

	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy_s(buffer, sizeof(buffer), text.c_str());
	bool update = ImGui::InputText("##InputText", buffer, sizeof(buffer),
		ImGuiInputTextFlags_EnterReturnsTrue);
	if (update)
		text = std::string(buffer);
	
	ImGui::Columns(1);
	ImGui::PopID();
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
	ImGui::Columns(1);
	ImGui::PopID();
}

bool Engine::ImGuiUtil::Button(const std::string& label, const std::string& buttonLable, float columnWidth)
{
	ImGui::PushID(label.c_str());
	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(0);
	bool update = ImGui::Button(buttonLable.c_str());
	ImGui::Columns(1);
	ImGui::PopID();
	return update;
}

bool Engine::ImGuiUtil::Button(const std::string& label, const std::string& buttonLable1, bool& button1, const std::string& buttonLable2, bool& button2, float columnWidth)
{
	ImGui::PushID(label.c_str());
	ImGui::Columns(2, 0, false);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(0);
	button1 = ImGui::Button(buttonLable1.c_str());
	ImGui::SameLine();
	button2 = ImGui::Button(buttonLable2.c_str());
	ImGui::Columns(1);
	ImGui::PopID();
	return button1 | button2;
}

void Engine::ImGuiUtil::HeaderText(const std::string& text)
{
	ImGuiIO& io = ImGui::GetIO();
	auto headerFont = io.Fonts->Fonts[2];
	ImGui::PushFont(headerFont);
	ImGui::Text(text.c_str());
	ImGui::PopFont();
}

void Engine::ImGuiUtil::BoldText(const std::string& text)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[1];
	ImGui::PushFont(boldFont);
	ImGui::Text(text.c_str());
	ImGui::PopFont();
}
