#include "dypch.h"
#include "Nodes.hpp"

#include "Dynamite/Core/Logging.hpp"

#include "Dynamite/Tokens/Tokenizer.hpp"

#include <Pulse/Types/TypeUtils.hpp>

namespace Dynamite::Nodes
{

	// Note: This function has to be manually updated
	std::string ExpressionToString(const Expression& expr)
	{
		return std::visit([](auto&& obj) -> std::string
		{
			using namespace Pulse::Types;

			if constexpr (Same<Clean<decltype(obj)>, Identifier>)			return "Identifier";
			else if constexpr (Same<Clean<decltype(obj)>, Int64Literal>)	return "Int64Literal";

			return "Unspecified Expression Type";
		}, expr);
	}

	// Note: This function has to be manually updated
	std::string FormatExpressionData(const Expression& expr)
	{
		return std::visit([](auto&& obj) -> std::string
		{
			using namespace Pulse::Types;

			if constexpr (Same<Clean<decltype(obj)>, Identifier>)
				return Tokenizer::FormatToken(obj.TokenObject);
			else if constexpr (Same<Clean<decltype(obj)>, Int64Literal>)
				return Tokenizer::FormatToken(obj.TokenObject);

			return "Invalid Data";
		}, expr);
	}

	// Note: This function has to be manually updated
	std::string StatementToString(const Statement& statement)
	{
		return std::visit([](auto&& obj) -> std::string
		{
			using namespace Pulse::Types;

			if constexpr (Same<Clean<decltype(obj)>, LetStatement>)			return "LetStatement";
			else if constexpr (Same<Clean<decltype(obj)>, ExitStatement>)	return "ExitStatement";

			return "Unspecified Statement Type";
		}, statement);
	}

	// Note: This function has to be manually updated
	std::string FormatStatementData(const Statement& statement)
	{
		return std::visit([](auto&& obj) -> std::string
		{
			using namespace Pulse::Types;

			if constexpr (Same<Clean<decltype(obj)>, LetStatement>)			
				return Pulse::Text::Format("{0}([{1}])", Tokenizer::FormatToken(obj.TokenObject), FormatExpressionData(obj.ExpressionObject));
			else if constexpr (Same<Clean<decltype(obj)>, ExitStatement>)
				return Pulse::Text::Format("{0}", FormatExpressionData(obj.ExpressionObject));

			return "Unspecified Statement Type";
		}, statement);
	}

}