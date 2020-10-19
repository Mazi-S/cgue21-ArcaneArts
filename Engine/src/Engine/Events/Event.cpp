#include "egpch.h"
#include "Event.h"

uint32_t Engine::operator|(EventCategory lhs, EventCategory rhs)
{
	return static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs);
}

uint32_t Engine::operator|(uint32_t lhs, EventCategory rhs)
{
	return lhs | static_cast<uint32_t>(rhs);
}

uint32_t Engine::operator|(EventCategory lhs, uint32_t rhs)
{
	return static_cast<uint32_t>(lhs) | rhs;
}
