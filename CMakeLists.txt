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

add_executable(social_network_backend
    src/main.cpp
    src/database/Database.cpp
    src/repositories/UserRepository.cpp
    src/repositories/PostRepository.cpp
    src/repositories/CommentRepository.cpp 
    src/repositories/LikeRepository.cpp 
    src/repositories/FollowRequestRepository.cpp 
    src/repositories/FollowRepository.cpp 
    src/utils/DateTimeUtils.cpp
    src/utils/UserMapper.cpp "src/utils/FollowRequestStatusUtils.cpp")

target_include_directories(social_network_backend
    PRIVATE
        "${MYSQL_CONCPP_ROOT}/include"
)

target_link_libraries(social_network_backend
    PRIVATE
        "${MYSQL_CONCPP_ROOT}/lib64/vs14/mysqlcppconnx.lib"
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