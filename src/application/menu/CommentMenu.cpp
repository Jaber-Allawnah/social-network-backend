#include "CommentMenu.hpp"
#include "../../utils/DateTimeUtils.hpp"
#include "../../utils/MenuInputUtils.hpp"
#include <iostream>

using namespace DateTimeUtils;
using namespace MenuInputUtils;

CommentMenu::CommentMenu(CommentService& commentService) : commentService_(commentService) {}

void CommentMenu::displayComment(const Comment& comment) const {
    std::cout << "Comment ID: " << comment.id
              << " | Post ID: " << comment.postId
              << " | User ID: " << comment.userId
              << " | Content: " << comment.content
              << " | Created At: " << formatDateTime(comment.createdAt)
              << " | Updated At: " << formatDateTime(comment.updatedAt)
              << '\n';
}

void CommentMenu::displayMenu() const{
    std::cout << "\nComments\n"
              << "1. Add Comment\n"
              << "2. Update Comment\n"
              << "3. Delete Comment\n"
              << "4. View Comments on a Post\n"
              << "5. Back\n"
              << "Enter your choice: ";
}

void CommentMenu::addComment(int currentUserId) {
    int postId = readId("Enter the ID of the post you want to comment on: ");
    std::string content = readNonEmptyLine("Enter your comment: ", 
                                           "Comment cannot be empty. Please try again: ");

    commentService_.create(currentUserId, postId, content);
    std::cout << "Comment added successfully.\n";
}

void CommentMenu::updateComment(int currentUserId) {
    int postId = readId("Enter the ID of the post that contains the comment: ");
    int commentId = readId("Enter the ID of the comment you want to update: ");
    std::string content = readNonEmptyLine("Enter the new comment content: ",
                                           "Comment cannot be empty. Please try again: ");

    if (commentService_.update(currentUserId, commentId, postId, content)) {
        std::cout << "Comment updated successfully.\n";
    }
}

void CommentMenu::deleteComment(int currentUserId) {
    int postId = readId("Enter the ID of the post that contains the comment: ");
    int commentId = readId("Enter the ID of the comment you want to delete: ");

    if (commentService_.remove(currentUserId, commentId, postId)) {
        std::cout << "Comment deleted successfully.\n";
    }
}

void CommentMenu::viewComments(int currentUserId) {
    int postId = readId("Enter the ID of the post: ");

    std::vector<Comment> comments = commentService_.getPostComments(postId, currentUserId);
    if (comments.empty()) {
        std::cout << "This post has no comments.\n";
        return;
    }

    for (const Comment& comment : comments) {
        displayComment(comment);
    }
}

void CommentMenu::handleChoice(int choice, int currentUserId) {
    switch (choice) {
    case 1:
        addComment(currentUserId);
        break;
    case 2:
        updateComment(currentUserId);
        break;
    case 3:
        deleteComment(currentUserId);
        break;
    case 4:
        viewComments(currentUserId);
        break;
    case 5:
        break;
    default:
        std::cout << "Please enter a valid option.\n";
        break;
    }
}
