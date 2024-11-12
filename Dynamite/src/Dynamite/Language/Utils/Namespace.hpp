#pragma once

#include "Dynamite/Core/Core.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Namespace
	/////////////////////////////////////////////////////////////////
	class Namespace
	{
	public:
		// Constructors
		Namespace() = default;
		Namespace(const std::vector<std::string>& levels);
		~Namespace() = default;

		// Methods
		void AddLevel(const std::string& name);
		void PopLevels(size_t count = 1);

		std::string GetTopLevel() const;
		const std::vector<std::string>& GetAllLevels() const;

		inline size_t Size() const { return m_NamespaceLevels.size(); }

		// Operators
		bool operator == (const Namespace& other) const;
		bool operator != (const Namespace& other) const;

	private:
		std::vector<std::string> m_NamespaceLevels = {};
	};

}