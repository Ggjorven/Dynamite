#include "dypch.h"
#include "ParserNamespaceCollection.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Utils.hpp"
#include "Dynamite/Utils/Checks.hpp"

#include "Dynamite/Compiler/Compiler.hpp"

namespace Dynamite
{

	using namespace Language;

	/////////////////////////////////////////////////////////////////
	// ParserNamespaceCollection
	/////////////////////////////////////////////////////////////////
	void ParserNamespaceCollection::Reset()
	{
		s_Namespaces.clear();
	}

	void ParserNamespaceCollection::PushNamespace(const ParserNamespace& nameSpace)
	{
		if (Exists(nameSpace))
			return;

		s_Namespaces.push_back(nameSpace);
	}

	bool ParserNamespaceCollection::Exists(const ParserNamespace& nameSpace)
	{
		for (const auto& ns : s_Namespaces)
		{
			if (nameSpace.Namespaces == ns.Namespaces)
				return true;
		}

		return false;
	}

}