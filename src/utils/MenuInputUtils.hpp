#pragma once

#include <string>

namespace MenuInputUtils {
	int readId(const std::string& prompt);

	std::string readNonEmptyLine(const std::string& prompt,
								 const std::string& emptyMessage);
}