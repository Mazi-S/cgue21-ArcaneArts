#pragma once
#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Engine {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};

}

#define LOG_TRACE(...)         Engine::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)          Engine::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)          Engine::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)         Engine::Log::GetLogger()->error(__VA_ARGS__)

