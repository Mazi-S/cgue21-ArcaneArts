#pragma once
#include <stdint.h>
#include <set>

namespace Engine {

	class Identifier
	{
	public:
		static uint32_t newID();

		// returns the given id or a new one if the given id is already used
		static uint32_t use(uint32_t id);

	private:
		static uint32_t randomID();
		static bool isUsed(uint32_t id);

	private:
		static std::set<uint32_t> s_Used;
	};

}