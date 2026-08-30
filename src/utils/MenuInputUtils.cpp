#include "MenuInputUtils.hpp"
#include <iostream>
#include <limits>
#include <spdlog/spdlog.h>

namespace MenuInputUtils {
	int readId(const std::string& prompt) {
		spdlog::debug("Reading ID from user input");

		int id;
		std::cout << prompt;
		while (!(std::cin >> id) || id <= 0) {
			spdlog::warn("Invalid ID entered by user");

			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			std::cout << "Invalid input. Please enter a valid ID: ";
		}
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		spdlog::debug("Valid ID {} read successfully", id);

		return id;
	}

	std::string readNonEmptyLine(const std::string& prompt,
		const std::string& emptyMessage) {
		spdlog::debug("Reading non-empty text input from user");

		std::string content;
		std::cout << prompt;
		std::getline(std::cin, content);

		while (content.empty()) {
			spdlog::warn("Empty text input entered by user");

			std::cout << emptyMessage;
			std::getline(std::cin, content);
		}

		spdlog::debug("Non-empty text input read successfully");

		return content;
	}
}