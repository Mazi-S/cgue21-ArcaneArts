#pragma once
#include <string>
#include <glm/glm.hpp>

namespace Engine {

	class ImGuiUtil
	{
	public:
		static bool DrawComboControl(const std::string& label, int& selectionIndex, const std::vector<std::string> values, float columnWidth = 100.0f);
		static bool DrawComboControl(const std::string& label, std::string& selection, const std::vector<std::string> values, float columnWidth = 100.0f);

		static bool DrawFloatControl(const std::string& label, float& value, float min = 0, float max = 0, float speed = 0.1f, float columnWidth = 100.0f);
		static bool DrawFloat3Control(const std::string& label, glm::vec3& values, float min = 0, float max = 0, float speed = 0.1f, float columnWidth = 100.0f);

		static bool DrawColorControl(const std::string& label, glm::vec4& values, float columnWidth = 100.0f);
		static bool DrawColorControl(const std::string& label, glm::vec3& values, float columnWidth = 100.0f);

		static bool InputText(const std::string& label, std::string& text, float columnWidth = 100.0f);

		static void Text(const std::string& label, const std::string& text, float columnWidth = 100.0f);

		static bool Button(const std::string& label, const std::string& buttonLable, float columnWidth = 100.0f);

		static bool Button(const std::string& label, const std::string& buttonLable1, bool& button1, const std::string& buttonLable2, bool& button2, float columnWidth = 100.0f);

		static void HeaderText(const std::string& text);
		static void BoldText(const std::string& text);
	};

}