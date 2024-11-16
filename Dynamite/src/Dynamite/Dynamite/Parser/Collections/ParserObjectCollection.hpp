#pragma once

#include "Dynamite/Core/Core.hpp"

#include "Dynamite/Language/Types/Type.hpp"

#include "Dynamite/Language/AccessSpecifier.hpp"
#include "Dynamite/Language/Utils/Namespace.hpp"

#include "Dynamite/Parser/Collections/ParserScopeCollection.hpp"
#include "Dynamite/Parser/Collections/ParserFunctionCollection.hpp"

#include <cstdint>
#include <string>
#include <vector>
#include <utility>

namespace Dynamite
{

	// TODO: Fully implement...
	// TODO: Fully implement...
	// TODO: Fully implement...

	/////////////////////////////////////////////////////////////////
	// Structs
	/////////////////////////////////////////////////////////////////
	enum class ParserObjectType
	{
		None = 0,

		Class,
		Struct
	};

	struct ParserMemberVariable
	{
	public:
		Language::AccessSpecifier Access;

		// Note: Namespaces must be empty
		ParserVariable Variable;
	};

	struct ParserMemberFunction
	{
	public:
		Language::AccessSpecifier Access;

		// Note: Namespaces must be empty
		ParserFunction Function;
	};

	struct ParserObject
	{
	public:
		ParserObjectType ObjectType = ParserObjectType::None;

		Language::Namespace Namespaces;
		std::string Identifier;

		std::vector<ParserMemberVariable> Variables;
		std::vector<ParserMemberFunction> Functions;
	};

	/////////////////////////////////////////////////////////////////
	// ParserObjectCollection
	/////////////////////////////////////////////////////////////////
	class ParserObjectCollection
	{
	public:
		//static void Reset();


	private:
		//inline static std::vector<ParserNamespace> s_Namespaces = {};
	};

}