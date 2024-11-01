#pragma once

#include <Pulse/Core/Ref.hpp>
#include <Pulse/Core/Unique.hpp>
#include <Pulse/Core/WeakRef.hpp>

#include <Pulse/Utility/Optional.hpp>

#include <Pulse/Memory/Memory.hpp>
#include <Pulse/Memory/ArenaAllocator.hpp>

namespace Dynamite
{

	namespace Utility
	{
		template<typename T>
		using Optional = Pulse::Utility::Optional<T>;
	}

	// Note: We use them as to not have to use <prefix>::
	using namespace Utility;

}