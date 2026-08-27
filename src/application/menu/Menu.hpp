#pragma once

class Menu {
private:
	int readChoice() const;

protected:
	virtual void handleChoice(int choice, int currentUserId) = 0;
	virtual void displayMenu() const = 0;

public:
	virtual void run(int currentUserId) final;
	virtual ~Menu() = default;
};