#include "dypch.h"
#include "Namespace.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Utils/Checks.hpp"

#include <Pulse/Core/Defines.hpp>

#undef FMT_VERSION
#include <Pulse/Enum/Enum.hpp>

namespace Dynamite::Language
{

	/////////////////////////////////////////////////////////////////
	// Namespace
	/////////////////////////////////////////////////////////////////
	// Constructors
	Namespace::Namespace(const std::vector<std::string>& levels)
		: m_NamespaceLevels(levels)
	{
	}

	// Methods
	void Namespace::AddLevel(const std::string& name)
	{
		m_NamespaceLevels.push_back(name);
	}

	void Namespace::PopLevels(size_t count)
	{
		if (count > m_NamespaceLevels.size())
		{
			DY_LOG_ERROR("[Internal Compiler Error] - PopLevels(count) is more than namespace size.");
			m_NamespaceLevels.clear();
		}
		else
			m_NamespaceLevels.resize(m_NamespaceLevels.size() - count);
	}

	std::string Namespace::GetTopLevel() const
	{
		if (m_NamespaceLevels.empty())
			return {};

		return m_NamespaceLevels.back();
	}

	const std::vector<std::string>& Namespace::GetAllLevels() const
	{
		return m_NamespaceLevels;
	}

	// Operators
	bool Namespace::operator == (const Namespace& other) const
	{
		if (this->m_NamespaceLevels.size() != other.m_NamespaceLevels.size())
			return false;

		for (size_t i = 0; i < this->m_NamespaceLevels.size(); i++)
		{
			if (this->m_NamespaceLevels[i] != other.m_NamespaceLevels[i])
				return false;
		}

		return true;
	}

	bool Namespace::operator != (const Namespace& other) const
	{
		return !(*this == other);
	}

}