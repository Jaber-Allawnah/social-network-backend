#include "Menu.hpp"
#include <iostream>
#include <limits>

int Menu::readChoice() const {
    int choice;

    while (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

        std::cout << "Invalid input. Please enter a number: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return choice;
}

void Menu::run(int currentUserId) {
	displayMenu();
	int choice = readChoice();
	handleChoice(choice, currentUserId);
}