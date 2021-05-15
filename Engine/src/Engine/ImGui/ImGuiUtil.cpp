#include "egpch.h"
#include "ImGuiUtil.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "Engine/Util/FileDialog.h"

namespace Engine::ImGuiUtil {

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Style //////////////////////////////////////////////////////////////////////////////////////

	void PushButtonStyle(ButtonType type)
	{
		if (type == ButtonType::Default)
			return;

		if (type == ButtonType::Success)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.19, 0.49, 0.29, 1 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.35, 0.62, 0.44, 1 });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.15, 0.38, 0.22, 1 });
		}

		if (type == ButtonType::Danger)
		{
			ImGui::PushStyleColor(ImGuiCol_Button, { 0.60, 0.21, 0.27, 1 });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.78, 0.32, 0.41, 1 });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.49, 0.18, 0.24, 1 });
		}
	}

	void PopButtonStyle(ButtonType type)
	{
		if (type == ButtonType::Default)
			return;

		ImGui::PopStyleColor(3);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// UI /////////////////////////////////////////////////////////////////////////////////////////

	bool DrawComboControl(const std::string& label, int& selectionIndex, const std::vector<std::string> values, float columnWidth)
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

	bool DrawComboControl(const std::string& label, std::string& selection, const std::vector<std::string> values, float columnWidth)
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

	bool DrawFloatControl(const std::string& label, float& value, float min, float max, float speed, float columnWidth)
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

	bool DrawFloat3Control(const std::string& label, glm::vec3& values, float min, float max, float speed, float columnWidth)
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

	bool DrawColorControl(const std::string& label, glm::vec4& values, float columnWidth)
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

	bool DrawColorControl(const std::string& label, glm::vec3& values, float columnWidth)
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

	bool InputText(const std::string& label, std::string& text, float columnWidth)
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

	static std::string GetPath(const char* filter)
	{
		std::string path = FileDialog::OpenFile(filter);
		std::size_t pos = path.find("assets");

		if (pos == std::string::npos)
			return std::string();

		return path.substr(pos);
	}

	bool InputPath(const std::string& label, std::string& path, const char* filter, float columnWidth)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(0);

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), path.c_str());
		bool update = false;

		if (ImGui::InputText("##InputText", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			path = std::string(buffer);
			update = true;
		}

		ImGui::SameLine();

		if (update = ImGui::Button("..."))
		{
			std::string newPath = GetPath(filter);
			if (!newPath.empty())
			{
				path = newPath;
				update = true;
			}
		}

		ImGui::Columns(1);
		ImGui::PopID();
		return update;
	}

	void Text(const std::string& label, const std::string& text, float columnWidth)
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

	bool Button(const std::string& label, const std::string& buttonLable, ButtonType type, float columnWidth)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(0);
		PushButtonStyle(type);
		bool update = ImGuiUtil::Button(buttonLable.c_str());
		PopButtonStyle(type);
		ImGui::Columns(1);
		ImGui::PopID();
		return update;
	}

	bool Button(const std::string& label, const std::string& buttonLable1, bool& button1, ButtonType type1, const std::string& buttonLable2, bool& button2, ButtonType type2, float columnWidth)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(0);
		button1 = ImGuiUtil::Button(buttonLable1.c_str(), glm::vec2{ 0, 0 }, type1);
		ImGui::SameLine();
		button2 = ImGuiUtil::Button(buttonLable2.c_str(), glm::vec2{ 0, 0 }, type2);
		ImGui::Columns(1);
		ImGui::PopID();
		return button1 | button2;
	}

	bool Checkbox(const std::string& label, bool& value, float columnWidth)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2, 0, false);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::PushItemWidth(0);
		bool update = ImGui::Checkbox("", &value);
		ImGui::Columns(1);
		ImGui::PopID();
		return update;
	}

	bool Button(const std::string& label, glm::vec2 size, ButtonType type)
	{
		ImGui::PushID(label.c_str());
		PushButtonStyle(type);
		bool update = ImGui::Button(label.c_str(), { size.x, size.y });
		PopButtonStyle(type);
		ImGui::PopID();
		return update;
	}

	void HeaderText(const std::string& text)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto headerFont = io.Fonts->Fonts[2];
		ImGui::PushFont(headerFont);
		ImGui::Text(text.c_str());
		ImGui::PopFont();
	}

	void BoldText(const std::string& text)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];
		ImGui::PushFont(boldFont);
		ImGui::Text(text.c_str());
		ImGui::PopFont();
	}

}