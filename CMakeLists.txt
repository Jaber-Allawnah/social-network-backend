cmake_minimum_required(VERSION 3.20)

project(SocialNetworkBackend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(
    MYSQL_CONCPP_ROOT
    "C:/Program Files/MySQL/MySQL Connector C++ 26.7"
    CACHE PATH "Path to MySQL Connector/C++ installation"
)

set(
    LIBSODIUM_ROOT
    "C:/Users/Hp/Desktop/libsodium-1.0.22-stable-msvc/libsodium"
    CACHE PATH "Path to libsodium installation"
)

set(
    SPDLOG_ROOT
    "C:/Users/Hp/.conan2/p/b/spdloa78a2210c1da2/b/src"
    CACHE PATH "Path to spdlog installation"
)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(social_network_backend
    src/main.cpp

    src/database/Database.cpp
    src/database/TransactionManager.cpp

    src/repositories/UserRepository.cpp
    src/repositories/PostRepository.cpp
    src/repositories/CommentRepository.cpp
    src/repositories/LikeRepository.cpp
    src/repositories/FollowRequestRepository.cpp
    src/repositories/FollowRepository.cpp

    src/utils/DateTimeUtils.cpp
    src/utils/UserMapper.cpp
    src/utils/FollowRequestStatusUtils.cpp
    src/utils/PasswordHasher.cpp
    src/utils/MenuInputUtils.cpp
    src/utils/Logger.cpp

    src/services/UserService.cpp
    src/services/PostService.cpp
    src/services/CommentService.cpp
    src/services/LikeService.cpp
    src/services/FollowService.cpp

    src/application/Application.cpp
    src/application/LoggedInMenu.cpp
    src/application/LoggedOutMenu.cpp

    src/application/menu/Menu.cpp
    src/application/menu/PostMenu.cpp
    src/application/menu/CommentMenu.cpp
    src/application/menu/LikeMenu.cpp
    src/application/menu/FollowMenu.cpp
    src/application/menu/ConnectionMenu.cpp)

target_include_directories(social_network_backend
    PRIVATE
        "${MYSQL_CONCPP_ROOT}/include"
        "${LIBSODIUM_ROOT}/include"
        "${SPDLOG_ROOT}/include"
)

target_compile_definitions(social_network_backend
    PRIVATE
        SODIUM_STATIC=1
        SODIUM_EXPORT=
)

target_compile_options(social_network_backend
    PRIVATE
        /utf-8
)

target_link_libraries(social_network_backend
    PRIVATE
        "${MYSQL_CONCPP_ROOT}/lib64/vs14/mysqlcppconnx.lib"
        "${LIBSODIUM_ROOT}/x64/Release/v143/static/libsodium.lib"
)

add_custom_command(
    TARGET social_network_backend
    POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MYSQL_CONCPP_ROOT}/lib64/mysqlcppconnx-2-vs14.dll"
        "$<TARGET_FILE_DIR:social_network_backend>"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MYSQL_CONCPP_ROOT}/lib64/libssl-3-x64.dll"
        "$<TARGET_FILE_DIR:social_network_backend>"

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MYSQL_CONCPP_ROOT}/lib64/libcrypto-3-x64.dll"
        "$<TARGET_FILE_DIR:social_network_backend>"
)

enable_testing()

add_executable(service_tests
    tests/TestMain.cpp

    tests/services/PostServiceTest.cpp
    tests/services/FollowServiceTest.cpp
    tests/services/LikeServiceTest.cpp
    tests/services/CommentServiceTest.cpp

    src/services/PostService.cpp
    src/services/FollowService.cpp
    src/services/LikeService.cpp
    src/services/CommentService.cpp
)
    
target_include_directories(service_tests
    PRIVATE
        "${SPDLOG_ROOT}/include"
)

target_compile_options(service_tests
    PRIVATE
        /utf-8
)

target_link_libraries(service_tests
    PRIVATE
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(service_tests)