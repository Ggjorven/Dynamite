#pragma once

#include "Dynamite/Core/Core.hpp"

#include <cstdint>
#include <vector>

namespace Dynamite::Utils
{

	// Optional checking utilities
	template<typename T>
	inline bool OptCheck(const Optional<T>& opt, T check)
	{
		return opt.HasValue() && (opt.Value() == check);
	}

	template<typename T, typename Func, typename ...Args>
	inline bool OptCheck(const Optional<T>& opt, Func&& func, Args&& ...args)
	{
		return opt.HasValue() && func(opt.Value(), std::forward<Args>(args)...);
	}

	template<typename T, typename TMember>
	inline bool OptMemberIs(const Optional<T>& opt, TMember T::* member, TMember value)
	{
		return opt.HasValue() && (opt.Value().*member == value);
	}

	template<typename T, typename TMember>
	inline Optional<T> OptReturnIfMemberIs(const Optional<T>& opt, TMember T::* member, TMember value)
	{
		if (OptMemberIs(opt, member, value))
		{
			return opt;
		}

		return {};
	}

	template<typename T, typename TMember>
	inline Optional<T> OptReturnIfMemberIs(const Optional<T>& opt, TMember T::* member, const std::vector<TMember>& allowedValues)
	{
		for (const auto& value : allowedValues)
		{
			if (OptMemberIs(opt, member, value))
			{
				return opt;
			}
		}

		return {};
	}

}