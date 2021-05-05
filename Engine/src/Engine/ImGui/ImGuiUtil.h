#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "Platform/OpenGL/OpenGLTexture.h"

namespace Engine::ImGuiUtil {

	enum class ButtonType
	{
		Default = 0,
		Success,
		Danger
	};

	void PushButtonStyle(ButtonType type);
	void PopButtonStyle(ButtonType type);


	bool DrawComboControl(const std::string& label, int& selectionIndex, const std::vector<std::string> values, float columnWidth = 100.0f);
	bool DrawComboControl(const std::string& label, std::string& selection, const std::vector<std::string> values, float columnWidth = 100.0f);

	bool DrawFloatControl(const std::string& label, float& value, float min = 0, float max = 0, float speed = 0.1f, float columnWidth = 100.0f);
	bool DrawFloat3Control(const std::string& label, glm::vec3& values, float min = 0, float max = 0, float speed = 0.1f, float columnWidth = 100.0f);

	bool DrawColorControl(const std::string& label, glm::vec4& values, float columnWidth = 100.0f);
	bool DrawColorControl(const std::string& label, glm::vec3& values, float columnWidth = 100.0f);

	bool InputText(const std::string& label, std::string& text, float columnWidth = 100.0f);

	void Text(const std::string& label, const std::string& text, float columnWidth = 100.0f);

	bool Button(const std::string& label, const std::string& buttonLable, ButtonType type1 = ButtonType::Default, float columnWidth = 100.0f);
	bool Button(const std::string& label, const std::string& buttonLable1, bool& button1, const std::string& buttonLable2, bool& button2, float columnWidth = 100.0f);

	bool Button(const std::string& label, glm::vec2 size = { 0, 0 }, ButtonType type = ButtonType::Default);

	void HeaderText(const std::string& text);
	void BoldText(const std::string& text);

}