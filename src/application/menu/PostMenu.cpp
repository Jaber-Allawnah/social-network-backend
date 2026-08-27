#include "PostMenu.hpp"
#include "../../utils/DateTimeUtils.hpp"
#include "../../utils/MenuInputUtils.hpp"
#include <iostream>

using namespace MenuInputUtils;
using namespace DateTimeUtils;

PostMenu::PostMenu(PostService& postService) : postService_(postService) {}

void PostMenu::displayPost(const Post& post) const {
    std::cout << "Post ID: " << post.id
              << " | User ID: " << post.userId
              << " | Content: " << post.content
              << " | Created At: " << formatDateTime(post.createdAt)
              << " | Updated At: " << formatDateTime(post.updatedAt)
              << '\n';
}

void PostMenu::displayMenu() const {
    std::cout << "\nPosts Menu\n"
              << "1. Create Post\n"
              << "2. Update Post\n"
              << "3. Delete Post\n"
              << "4. View Post\n"
              << "5. View My Posts\n"
              << "6. View Another User's Posts\n"
              << "7. Search User's Posts\n"
              << "8. Back\n"
              << "Enter your choice: ";
}

void PostMenu::createPost(int currentUserId) {
    std::string content = readNonEmptyLine("Enter post content: ",
                                           "Post content cannot be empty. Please try again: ");

    displayPost(postService_.create(currentUserId, content));
    std::cout << "Post created successfully.\n";
}

void PostMenu::updatePost(int currentUserId) {
    int postId = readId("Enter the ID of the post you want to update: ");

    std::string content = readNonEmptyLine("Enter the new post content: ",
                                           "Post content cannot be empty. Please try again: ");

    if(postService_.update(postId, currentUserId, content))
        std::cout << "Post updated successfully.\n";
}

void PostMenu::deletePost(int currentUserId) {
    int postId = readId("Enter the ID of the post you want to delete: ");
    
    if(postService_.remove(postId, currentUserId))
        std::cout << "Post deleted successfully.\n";
}

void PostMenu::viewPost(int currentUserId) {
    int postId = readId("Enter the ID of the post you want to view: ");
   
    Post post = postService_.getPost(postId, currentUserId);
    displayPost(post);
}

void PostMenu::viewMyPosts(int currentUserId) {
    std::vector<Post> posts = postService_.getUserPosts(currentUserId, currentUserId);

    if (posts.empty()) {
        std::cout << "You have no posts.\n";
        return;
    }

    for (const Post& post : posts) {
        displayPost(post);
    }
}

void PostMenu::viewUserPosts(int currentUserId) {
    int ownerId = readId("Enter the ID of the user whose posts you want to view: ");

    std::vector<Post> posts = postService_.getUserPosts(ownerId, currentUserId);
    if (posts.empty()) {
        std::cout << "This user has no posts.\n";
        return;
    }

    for (const Post& post : posts) {
        displayPost(post);
    }
}

void PostMenu::searchUserPosts(int currentUserId) {
    int ownerId = readId("Enter the ID of the user whose posts you want to search: ");

    std::string keyword = readNonEmptyLine("Enter a keyword to search for: ", 
                                           "Keyword cannot be empty. Please try again: ");

    std::vector<Post> posts = postService_.searchUserPosts(ownerId, currentUserId, keyword);
    if (posts.empty()) {
        std::cout << "No matching posts found.\n";
        return;
    }

    for (const Post& post : posts) {
        displayPost(post);
    }
}

void PostMenu::handleChoice(int choice, int currentUserId) {
    switch (choice) {
    case 1:
        createPost(currentUserId);
        break;
    case 2:
        updatePost(currentUserId);
        break;
    case 3:
        deletePost(currentUserId);
        break;
    case 4:
        viewPost(currentUserId);
        break;
    case 5:
        viewMyPosts(currentUserId);
        break;
    case 6:
        viewUserPosts(currentUserId);
        break;
    case 7:
        searchUserPosts(currentUserId);
        break;
    case 8:
        break;
    default:
        std::cout << "Please enter a valid option.\n";
        break;
    }
}

