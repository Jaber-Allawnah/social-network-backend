#pragma once

#include "Menu.hpp"
#include "../../services/CommentService.hpp"

class CommentMenu : public Menu {
private:
	CommentService& commentService_;
	void handleChoice(int choice, int currentUserId) override;
	void displayMenu() const override;

	void addComment(int currentUserId);
	void updateComment(int currentUserId);
	void deleteComment(int currentUserId);

	void viewComments(int currentUserId);
	void displayComment(const Comment& comment) const;

public:
	CommentMenu(CommentService& commentService);
};