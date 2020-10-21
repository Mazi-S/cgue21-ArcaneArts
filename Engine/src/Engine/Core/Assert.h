#pragma once

#include "Log.h"

#ifdef ENGINE_DEBUG
	#define EG_ENABLE_ASSERTS
#endif

#ifdef EG_ENABLE_ASSERTS
	//#define ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed"); __debugbreak(); } }
#else
	#define ASSERT(x, ...)
#endif
