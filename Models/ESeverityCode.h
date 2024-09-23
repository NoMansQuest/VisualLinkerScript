#ifndef SEVERITY_CODE_H_
#define SEVERITY_CODE_H_

#include <cstdint>
#include <stdexcept>
#include <string>

namespace VisualLinkerScript::Models
{
	/// @brief Represents severity of a violation
	enum class ESeverityCode : uint32_t
	{
		Error,
		Warning,
		Information,
	};

	/// @brief Maps severity to std::string.
	inline std::string MapSeverityToString(ESeverityCode code)
	{
		switch (code)
		{
			case ESeverityCode::Error: return "Error";
			case ESeverityCode::Warning: return "Warning";
			case ESeverityCode::Information: return "Information";

			default: 
				throw std::invalid_argument("Severity-code value not recognized.");
		}
	}
}

#endif
