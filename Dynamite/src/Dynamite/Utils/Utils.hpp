#pragma once

#include "Dynamite/Core/Core.hpp"

#include <cstdint>
#include <ranges>

namespace Dynamite::Utils
{

	// Str utilities
	std::string StrTimes(const std::string& str, size_t times);
	std::string& StrLower(std::string& val);
	std::string& EmplaceAfterIndentation(std::string& val, const std::string& place);
	std::string& RemoveFrontIndentation(std::string& val);

	// Vector utilities
	/*
	template<typename T>
	T* GetVectorElement(std::vector<T>& vec, const T& checkAgainst)
	{
		auto it = std::ranges::find_if(vec, [&](const T& val)
		{
			return val == checkAgainst;
		});

		if (it == vec.end())
			return nullptr;

		return &(*it);
	}

	template<typename T>
	const T* GetVectorElement(const std::vector<T>& vec, const T& checkAgainst)
	{
		const auto it = std::ranges::find_if(vec, [&](const T& val)
			{
				return val == checkAgainst;
			});

		if (it == vec.cend())
			return nullptr;

		return &(*it);
	}

	template<typename T, typename TMember>
	T* GetVectorElement(std::vector<T>& vec, TMember T::* member, const T& checkAgainst)
	{
		auto it = std::ranges::find_if(vec, [&](const T& val)
		{
			return val.*member == checkAgainst;
		});

		if (it == vec.end())
			return nullptr;

		return &(*it);
	}

	template<typename T, typename TMember>
	const T* GetVectorElement(const std::vector<T>& vec, TMember T::* member, const T& checkAgainst)
	{
		const auto it = std::ranges::find_if(vec, [&](const T& val)
		{
			return val.*member == checkAgainst;
		});

		if (it == vec.cend())
			return nullptr;

		return &(*it);
	}
	*/

}