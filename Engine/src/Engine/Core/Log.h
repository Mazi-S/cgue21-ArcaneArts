#pragma once
#include <iostream>

namespace Engine {

	class Log
	{
	public:

	private:

	};

}

//#define LOG_TRACE(...)         Engine::Log::GetClientLogger()->trace(__VA_ARGS__)
//#define LOG_INFO(...)          Engine::Log::GetClientLogger()->info(__VA_ARGS__)
//#define LOG_WARN(...)          Engine::Log::GetClientLogger()->warn(__VA_ARGS__)
//#define LOG_ERROR(...)         Engine::Log::GetClientLogger()->error(__VA_ARGS__)

#define LOG_TRACE(msg)           std::cout << "TRACE:\t" << msg << std::endl;
#define LOG_INFO(msg)            std::cout << "INFO:\t"  << msg << std::endl;
#define LOG_WARN(msg)            std::cout << "WARN:\t"  << msg << std::endl;
#define LOG_ERROR(msg)           std::cout << "ERROR:\t" << msg << std::endl;


