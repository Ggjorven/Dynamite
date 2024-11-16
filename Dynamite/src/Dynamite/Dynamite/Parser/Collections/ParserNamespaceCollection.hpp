#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include "Dynamite/Language/Utils/Namespace.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite
{

	/////////////////////////////////////////////////////////////////
	// Structs
	/////////////////////////////////////////////////////////////////
	struct ParserNamespace
	{
	public:
		Language::Namespace Namespaces;
	};

	/////////////////////////////////////////////////////////////////
	// ParserNamespaceCollection
	/////////////////////////////////////////////////////////////////
	class ParserNamespaceCollection
	{
	public:
		static void Reset();

		static void PushNamespace(const ParserNamespace& nameSpace);

		static bool Exists(const ParserNamespace& nameSpace);

	private:
		inline static std::vector<ParserNamespace> s_Namespaces = {};
	};

}