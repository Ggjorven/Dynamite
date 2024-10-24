#pragma once

#include <Pulse/Core/Ref.hpp>
#include <Pulse/Core/Unique.hpp>
#include <Pulse/Core/WeakRef.hpp>

#include <Pulse/Utility/Optional.hpp>

#include <Pulse/Memory/Memory.hpp>
#include <Pulse/Memory/ArenaAllocator.hpp>

namespace Dynamite
{

	namespace Core
	{
		template<typename T>
		using Ref = Pulse::Ref<T>;

		template<typename T>
		using Unique = Pulse::Unique<T>;

		template<typename T>
		using WeakRef = Pulse::WeakRef<T>;
	}

	namespace Utility
	{
		template<typename T>
		using Optional = Pulse::Utility::Optional<T>;
	}

	namespace Memory
	{
		// using Control = Pulse::Memory::Control;

		// using ArenaAllocator = Pulse::Memory::DynamicArenaAllocator;
	}

	// Note: We use them as to not have to use <prefix>::
	using namespace Core;
	using namespace Utility;
	using namespace Memory;

}