#include "MenuInputUtils.hpp"
#include <iostream>
#include <limits>

namespace MenuInputUtils {
	int readId(const std::string& prompt) {
		int id;
		std::cout << prompt;
		while (!(std::cin >> id) || id <= 0) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			std::cout << "Invalid input. Please enter a valid ID: ";
		}
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		return id;
	}

	std::string readNonEmptyLine(const std::string& prompt,
								 const std::string& emptyMessage) {
		std::string content;
		std::cout << prompt;
		std::getline(std::cin, content);

		while (content.empty()) {
			std::cout << emptyMessage;
			std::getline(std::cin, content);
		}

		return content;
	}
}