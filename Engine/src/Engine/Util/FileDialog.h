#pragma once
#include <string>

namespace Engine::FileDialog {

	// Returns empty string if canceled
	std::string OpenFile(const char* filter);

	// Returns empty string if canceled
	std::string SaveFile(const char* filter);

}