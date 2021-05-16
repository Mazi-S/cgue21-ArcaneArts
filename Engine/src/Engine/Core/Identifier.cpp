#include "egpch.h"
#include "Identifier.h"
#include <random>

namespace Engine {

	std::set<uint32_t> Identifier::s_Used = { 0 };

	uint32_t Engine::Identifier::newID()
	{
		return use(randomID());
	}

	uint32_t Engine::Identifier::use(uint32_t id)
	{
		if (isUsed(id))
			return newID();

		s_Used.insert(id);
		return id;
	}

	uint32_t Identifier::randomID()
	{
		std::random_device rand_dev;
		std::mt19937 generator(rand_dev());
		std::uniform_int_distribution<uint32_t> distr(0, UINT32_MAX);
		return distr(generator);
	}

	bool Identifier::isUsed(uint32_t id)
	{
		return s_Used.find(id) != s_Used.end();
	}

}