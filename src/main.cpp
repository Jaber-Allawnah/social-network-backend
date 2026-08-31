#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib> 
#include <sodium.h>
#include "database/Database.hpp"
#include "repositories/UserRepository.hpp"
#include "services/UserService.hpp"
#include "repositories/PostRepository.hpp"
#include "repositories/CommentRepository.hpp"
#include "repositories/LikeRepository.hpp"
#include "repositories/FollowRequestRepository.hpp"
#include "repositories/FollowRepository.hpp"
#include "services/PostService.hpp"
#include "services/CommentService.hpp"
#include "services/LikeService.hpp"
#include "services/FollowService.hpp"
#include "application/menu/PostMenu.hpp"
#include "application/menu/CommentMenu.hpp"
#include "application/menu/LikeMenu.hpp"
#include "application/menu/FollowMenu.hpp"
#include "application/menu/ConnectionMenu.hpp"
#include "application/LoggedOutMenu.hpp"
#include "application/LoggedInMenu.hpp"
#include "application/Application.hpp"
#include "utils/Logger.hpp"

std::string getEnvironmentVariable(const char* name) {
    const char* value = std::getenv(name);

    if (value == nullptr) {
        throw std::runtime_error(std::string("Missing environment variable: ") + name);
    }

    return std::string(value);
}

int main() {
    Logger::init();

    try {
        const std::string host = getEnvironmentVariable("DB_HOST");
        const unsigned port = static_cast<unsigned>(std::stoi(getEnvironmentVariable("DB_PORT")));
        const std::string username = getEnvironmentVariable("DB_USER");
        const std::string password = getEnvironmentVariable("DB_PASSWORD");
        const std::string databaseName = getEnvironmentVariable("DB_NAME");

        // Database
        Database database(host,
                          port,
                          username,
                          password,
                          databaseName);

        // Repositories
        UserRepository userRepository(database);
        PostRepository postRepository(database);
        CommentRepository commentRepository(database);
        LikeRepository likeRepository(database);
        FollowRequestRepository followRequestRepository(database);
        FollowRepository followRepository(database);

        // Services
        UserService userService(userRepository);

        PostService postService(postRepository,
                                followRepository,
                                userRepository);

        CommentService commentService(commentRepository,
                                      postRepository,
                                      followRepository,
                                      userRepository);

        LikeService likeService(likeRepository,
                                followRepository,
                                postRepository,
                                userRepository);

        FollowService followService(followRepository,
                                    followRequestRepository,
                                    userRepository,
                                    database);

        // Menus
        PostMenu postMenu(postService);
        CommentMenu commentMenu(commentService);
        LikeMenu likeMenu(likeService);
        FollowMenu followMenu(followService);
        ConnectionMenu connectionMenu(followService);
        LoggedOutMenu loggedOutMenu(userService, 
                                    connectionMenu);
        LoggedInMenu loggedInMenu(postMenu,
                                  commentMenu,
                                  likeMenu,
                                  followMenu,
                                  connectionMenu,
                                  postService);

        // Application
        Application application(
            loggedOutMenu,
            loggedInMenu
        );

        application.run();

    }
    catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
        return 1;
    }

    return 0;
}